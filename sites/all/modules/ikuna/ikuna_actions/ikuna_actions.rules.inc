<?php

function ikuna_actions_rules_action_info() {
  $actions = array();

  $actions['agregar_envios'] = array(
    'label' => t('Agregar Envíos'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      )
    ),
    'group' => t('Ikuna'),
    //'base' => 'commerce_checkout_action_send_account_email',
    //'access callback' => 'rules_system_integration_access',
  );

    $actions['enviar_sms_artesanos'] = array(
    'label' => t('Enviar SMS Artesanos'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order'),
      )
    ),
    'group' => t('Ikuna')
  );

  $actions['actualizar_zona_producto'] = array(
    'label' => t('Actualizar Zona Producto'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_product',
        'label' => t('Order'),
      )
    ),
    'group' => t('Ikuna')
  );

  $actions['commerce_checkout_complete'] = array(
    'label' => t('Complete checkout for an order'),
    'parameter' => array(
      'commerce_order' => array(
        'type' => 'commerce_order',
        'label' => t('Order in checkout'),
      ),  
    ),
    'group' => t('Commerce Checkout'),
    'callbacks' => array(
      'execute' => 'commerce_checkout_rules_complete_checkout',
    ),
  );

  return $actions;
}

function actualizar_zona_producto($commerce_product)
{
   // echo "<pre>";
   // print_r($commerce_product);
   // echo "</pre>";
   // die;
   // obtengo peso y dimensiones del producto
   $peso = $commerce_product->field_artesania_peso["und"][0]["weight"];
   // $alto = $commerce_product->field_artesania_peso["und"][0]["length"];
   // $ancho = $commerce_product->field_artesania_peso["und"][0]["length"];
   // $largo = $commerce_product->field_artesania_peso["und"][0]["length"];

   // Id del usuario que creó el producto
   $user_id = $commerce_product->uid; 
   
   // valores precio
   $precio_original = $commerce_product->original->commerce_price["und"][0]["amount"];
   $precio_form = $commerce_product->commerce_price["und"][0]["amount"];

   // cargar datos artesano
   $artesano = user_load($user_id);
   $comuna_artesano = $artesano->field_user_direccion["und"][0]["administrative_area"];

   // cargar costo promedio despacho a Stgo por zonas desde la comuna de artesano
   $costo_transporte_santiago = getCostoSantiago($peso, $comuna_artesano);

  if($precio_original != $precio_form || $commerce_product->field_artesania_transporte["und"][0]["value"] == '')
  {
    // asignar valores y guardar producto
    $commerce_product->commerce_price["und"][0]["amount"] = $precio_form + $costo_transporte_santiago;
  }

   $commerce_product->field_artesania_transporte["und"][0]["value"] = $costo_transporte_santiago;
   commerce_product_save($commerce_product);
}

function getCostoSantiago($peso, $comuna_artesano)
{
  // si no viene comuna devuelvo costo medio
  //if($comuna_artesano == '') return 32000;

  // Definir zona : intra, cerca, lejos
  $array_comunas = get_array_comunas();
  $zona = $array_comunas[$comuna_artesano]["zona"];
  // Definir indice del peso en array
  switch (TRUE) 
  {
    case ($peso > 15000):
      $indice_peso = 7;
    break;
    case ($peso > 10000):
      $indice_peso = 6;
    break;
    case ($peso > 6000):
      $indice_peso = 5;
    break;
    case ($peso > 3000):
      $indice_peso = 4;
    break;
    case ($peso > 1500):
      $indice_peso = 3;
    break;
    case ($peso > 1000):
      $indice_peso = 2;
    break;
    case ($peso > 500):
      $indice_peso = 1;
    break;
    case ($peso > 0):
      $indice_peso = 0;
    break;
  }

  //echo $peso. ", " .$indice_peso;

  // Intra
  $tabla_precios["intra"][0] = 2500; 
  $tabla_precios["intra"][1] = 2500; 
  $tabla_precios["intra"][2] = 2500; 
  $tabla_precios["intra"][3] = 2500; 
  $tabla_precios["intra"][4] = 3000; 
  $tabla_precios["intra"][5] = 3000; 
  $tabla_precios["intra"][6] = 3500; 
  $tabla_precios["intra"][7] = 250;

  // Cerca
  $tabla_precios["cerca"][0] = 3000; 
  $tabla_precios["cerca"][1] = 3500; 
  $tabla_precios["cerca"][2] = 3500; 
  $tabla_precios["cerca"][3] = 4000; 
  $tabla_precios["cerca"][4] = 6000; 
  $tabla_precios["cerca"][5] = 7500; 
  $tabla_precios["cerca"][6] = 8000; 
  $tabla_precios["cerca"][7] = 500;

  // Lejos
  $tabla_precios["lejos"][0] = 4500; 
  $tabla_precios["lejos"][1] = 6000; 
  $tabla_precios["lejos"][2] = 6500; 
  $tabla_precios["lejos"][3] = 9500; 
  $tabla_precios["lejos"][4] = 16500; 
  $tabla_precios["lejos"][5] = 25500; 
  $tabla_precios["lejos"][6] = 32000; 
  $tabla_precios["lejos"][7] = 1800;

  if ($indice_peso == 7) return $tabla_precios[$zona][$indice_peso - 1] + (ceil(($peso - 15000) / 1000) * $tabla_precios[$zona][$indice_peso]) ;

  return $tabla_precios[$zona][$indice_peso];

}

function enviar_sms_artesanos($commerce_order)
{
  global $user;
  if ($commerce_order->status != 'completed') return false;

  $codigo_pais = "569";
  $order_wrapper = entity_metadata_wrapper('commerce_order', $commerce_order);
  $shipping = $order_wrapper->commerce_customer_shipping->commerce_customer_address->value();
  $direccion = $shipping["thoroughfare"].", ".$shipping["locality"]." ,".get_comuna_by_codigo($shipping["administrative_area"]);

  // GET Shipping Service
  $result = db_query("SELECT * FROM {commerce_line_item} WHERE order_id = :id AND type = :type", array(
  ':id' => $commerce_order->order_id,':type' => 'shipping',
  ));

  $row = $result->fetchAssoc();
  $shipping_name = $row["line_item_label"];

  foreach($array_productos_by_seller = get_productos_by_seller($commerce_order) as $seller)
  {
      $titulo_productos_array = array();
      foreach ($seller["productos"] as $seller_product) 
      {
          $msg = "Venta Ikuna:".$seller_product["nombre"].".ID Producto:".$seller_product["id_producto"].".Cantidad:" . round($seller_product["cantidad"]). ".Despacho a: ". $shipping["name_line"] ."," . $direccion . " via " . $shipping_name . ". Envianos numero envio y depositaremos tu dinero. Detalles en tu email.";

          foreach ($seller["telefonos"] as $telefono) {
              // Preparar parámetros Web service de correo
              $requestParams = array(
              'in0' => 'ikuna',
              'in1' => 'ikuna123',
              'in2' => $codigo_pais.$telefono,
              'in3' => $msg
              );

              if($user->uid != 334) // webpay
              {
              	if (enviar_sms($requestParams) == 0)
              	{ 
                	guardar_sms($commerce_order, $seller_product["id_producto"], $seller_product["id_tienda"]);
              	}
              }
          }
      }
  }
}

function enviar_sms($requestParams)
{
      if($_SERVER['SERVER_NAME'] == 'local.ikuna.cl') return 0;

      $client = new SoapClient("http://ida.itdchile.cl/services/smsApiService?wsdl");
      $response = $client->sendSms($requestParams);

      //print_r($requestParams);
      //echo $response->out->entry[2]->value.": ".$response->out->entry[0]->value;
      return $response->out->entry[2]->value;
}

function guardar_sms($commerce_order, $id_producto, $id_tienda)
{
      $node = new stdClass();
      $node->type = 'sms';
      node_object_prepare($node);
      
      $body_text = 'SMS';
      $node->title    = 'SMS' . ' - ' . date('d-m-Y');
      $node->language = LANGUAGE_NONE;

      $node->{'field_sms_artesano'}[$node->language][0]['target_id'] = $id_tienda;
      $node->{'field_sms_producto'}[$node->language][0]['target_id'] = $id_producto;
      $node->{'field_sms_id_orden'}[$node->language][0]['target_id'] = $commerce_order->order_id;

      $node->body[$node->language][0]['value']   = $body_text;
      $node->body[$node->language][0]['summary'] = text_summary($body_text);
      $node->body[$node->language][0]['format']  = 'filtered_html';
      
      $path = 'content/programmatically_created_node_' . date('YmdHis');
      $node->path = array('alias' => $path);

      node_save($node);
}

function agregar_envios($commerce_order)
{

  $order_wrapper = entity_metadata_wrapper('commerce_order', $commerce_order);

  // Determine the shipping profile reference field name for the order.
  $field_name = commerce_physical_order_shipping_field_name($commerce_order);
  $shipping_profile = $order_wrapper->{$field_name}->value();

  
  // Prepare the shipping address for use in the request.
  if (!empty($order_wrapper->{$field_name}->commerce_customer_address)) {
    $shipping_address = $order_wrapper->{$field_name}->commerce_customer_address->value();
  }
  else {
    $shipping_address = addressfield_default_values();
  }
  $destino = $shipping_address["administrative_area"];

  // print_r($commerce_order);
  // die;
  foreach($array_productos_by_seller = get_productos_by_seller($commerce_order) as $seller)
  {
      $weight_seller = 0;
      $volume_seller = 0;
      $precio_envio = 0;
      $titulo_envio_array = array();
      $id_productos_array = array();
      
      foreach ($seller["productos"] as $seller_product) 
      {
         $weight_seller += $seller_product["weight"];
         $volume_seller += $seller_product["volume"];
         $origen_seller = $seller_product["origen"];
         $precio_envio += $seller_product["precio"];
         $titulo_envio_array[] = $seller_product["nombre"];
         $id_productos_array[] = $seller_product["id_producto"];
         $tienda_envio = $seller_product["tienda"];
      }

      $peso_volumetico = $volume_seller / 6;

      if($peso_volumetico > $weight_seller)
      {
        $weight_seller = $peso_volumetico;
      }

      // Llamar al Web Service
      $monto_seller = get_monto_paquete_correo($origen_seller, $destino, $weight_seller);

      // Ingresar a la bd Envio

      $body_text = 'This is the body text I want entered with the node.';
  
      $node = new stdClass();
      $node->type = 'envio';
      node_object_prepare($node);
      
      $titulo_envio = $tienda_envio. ": " .implode(", ",$titulo_envio_array);
      $node->title    = $titulo_envio . ' - ' . date('d-m-Y');
      $node->language = LANGUAGE_NONE;

      //$node->{'field_envio_track_number'}[$node->language][0]['value'] = 123456;
      //$node->{'field_envio_id_orden'}[$node->language][0]['value'] = implode(",",$id_productos_array);
      $node->{'field_envio_costo_shipping'}[$node->language][0]['amount']['value'] = $monto_seller;
      $node->{'field_envio_costo_shipping'}[$node->language][0]['currency_code']['value'] = "CLP";

      $node->{'field_envio_costo_productos'}[$node->language][0]['amount']['value'] = $precio_envio;
      $node->{'field_envio_costo_productos'}[$node->language][0]['currency_code']['value'] = "CLP";

      foreach ($id_productos_array as $id_producto_array) 
      {
        $node->{'field_envio_productos'}[$node->language][]['target_id'] = $id_producto_array;
      }

      $node->{'field_envio_id_orden'}[$node->language][0]['target_id'] = $commerce_order->order_id;

      $node->body[$node->language][0]['value']   = $body_text;
      $node->body[$node->language][0]['summary'] = text_summary($body_text);
      $node->body[$node->language][0]['format']  = 'filtered_html';
      
      $path = 'content/programmatically_created_node_' . date('YmdHis');
      $node->path = array('alias' => $path);

      node_save($node);
  }
}