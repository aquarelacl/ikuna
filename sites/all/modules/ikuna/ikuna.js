//jQuery wrapper
(function ($) {

	$(document).ready(

		function(){
			try
			{
				$("#edit-field-artesania-transporte-und-0-value").attr("readonly","readonly");
				$("#edit-field-artesania-ganancia-und-0-value").attr("readonly","readonly");
				$("#edit-field-artesania-ganancia-und-0-value").css({"border":"1px solid #ccc"});
				$("#edit-commerce-price").css({"width":"25%", "float":"left"});
				$("#edit-field-artesania-ganancia").css({"float":"left", "margin-left":"30px"});
				$("#edit-field-artesania-transporte").css({"float":"left", "margin":"0px 10px 0px 30px"});
				$("#edit-commerce-price-und-0-amount").keypress(

				// Validar puntos y comas
				function(e)
				{
					//alert(e.which);
					if(e.which == 46 || e.which == 44)
					{
						return false;
					}
				});

				// Calcular valor
				$("#edit-commerce-price-und-0-amount").keyup(function() 
				{
	  				var valor_ingresado = $(this).val().replace(" ","");
	  				if(!isNaN(valor_ingresado) && valor_ingresado != "")
	  				{
	  					var ganancia = valor_ingresado * 0.833;   // 20% ikuna, 1/1.2 
	  					//var ganancia = valor_ingresado * 0.715; // 40% ikuna, 1/1.4
	  					//ganancia = Math.round(ganancia / 10) * 10;
	  					ganancia = Math.round(Math.round(ganancia) / 10) * 10;
	  					$("#edit-field-artesania-ganancia-und-0-value").val(ganancia); 
	  				}

				});
			
				// Copy Shipping to Billing
				$("#edit-customer-profile-shipping input").focusout(
					function()
					{
						var id_billing = $(this).attr("id").replace("shipping","billing"); 
						$("#"+id_billing).val($(this).val());
					}
				);

				$("#edit-customer-profile-shipping-commerce-customer-address-und-0-administrative-area").change(
					function()
					{
						var id_billing = $(this).attr("id").replace("shipping","billing");
						var id_comuna = $(this).find(":selected").attr("value");
						console.log(id_billing,id_comuna);
						$("#"+id_billing+" option[value="+id_comuna+"]").attr("selected", true);

					}
					);
			
			}catch(e){
				console.log(e);
			}

		}

		);
})(jQuery);