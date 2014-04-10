<?php

/**
 * @file
 * The default format for adresses.
 */

$plugin = array(
  'title' => t('Address form (country-specific)'),
  'format callback' => 'addressfield_format_address_generate',
  'type' => 'address',
  'weight' => -100,
);

/**
 * Format callback.
 *
 * @see CALLBACK_addressfield_format_callback()
 */
function addressfield_format_address_generate(&$format, $address, $context = array()) {
  // We start with a reasonable default: a simple block format suitable
  // for international shipping. We extend it with country-specific heuristics
  // below.

  // The street block.
  $format['street_block'] = array(
    '#type' => 'addressfield_container',
    '#attributes' => array('class' => array('street-block')),
    '#weight' => 0,
  );
  $format['street_block']['thoroughfare'] = array(
    '#title' => t('Address 1'),
    '#tag' => 'div',
    '#attributes' => array('class' => array('thoroughfare')),
    '#size' => 30,
    // The #required will be automatically set to FALSE when processing.
    '#required' => TRUE,
  );
  $format['street_block']['premise'] = array(
    '#title' => t('Address 2'),
    '#tag' => 'div',
    '#attributes' => array('class' => array('premise')),
    '#size' => 30,
  );
  $format['locality_block'] = array(
    '#type' => 'addressfield_container',
    '#attributes' => array('class' => array('addressfield-container-inline', 'locality-block', 'country-' . $address['country'])),
    '#weight' => 50,
  );
  $format['locality_block']['#attached']['css'][] = drupal_get_path('module', 'addressfield') . '/addressfield.css';
  $format['locality_block']['postal_code'] = array(
    '#title' => t('Postal Code'),
    '#size' => 10,
    '#required' => TRUE,
    '#attributes' => array('class' => array('postal-code')),
  );
  $format['locality_block']['locality'] = array(
    '#title' => t('City'),
    '#size' => 30,
    '#required' => TRUE,
    '#prefix' => ' ',
    '#attributes' => array('class' => array('locality')),
  );
  $format['country'] = array(
    '#title' => t('Country'),
    '#options' => _addressfield_country_options_list(),
    '#required' => TRUE,
    '#attributes' => array('class' => array('country')),
    '#weight' => 100,
  );

  // Those countries do not seem to have a relevant postal code.
  if (in_array($address['country'], array('AF', 'AG', 'AL', 'AO', 'BB', 'BI', 'BJ', 'BO', 'BS', 'BW', 'BZ', 'CF', 'CG', 'CM', 'CO', 'DJ', 'DM', 'EG', 'ER', 'FJ', 'GD', 'GH', 'GM', 'GQ', 'GY', 'IE', 'KI', 'KM', 'KP', 'KY', 'LC', 'LY', 'ML', 'MR', 'NA', 'NR', 'RW', 'SB', 'SC', 'SL', 'SR', 'ST', 'TD', 'TG', 'TL', 'TO', 'TT', 'TV', 'TZ', 'UG', 'VC', 'VU', 'WS', 'ZW'))) {
    unset($format['locality_block']['postal_code']);

    // Remove the prefix from the first widget of the block.
    $element_children = element_children($format['locality_block']);
    $first_child = reset($element_children);
    unset($format['locality_block'][$first_child]['#prefix']);
  }

  // Those countries generally use their administrative division in postal addresses.
  if (in_array($address['country'], array('AR', 'AU', 'BR', 'BS', 'BY', 'BZ', 'CA', 'CN', 'DO', 'EG', 'ES', 'FJ', 'FM', 'GB', 'HN', 'ID', 'IE', 'IN', 'IT', 'JO', 'JP', 'KI', 'KN', 'KR', 'KW', 'KY', 'KZ', 'MX', 'MY', 'MZ', 'NG', 'NI', 'NR', 'NZ', 'OM', 'PA', 'PF', 'PG', 'PH', 'PR', 'PW', 'RU', 'SM', 'SO', 'SR', 'SV', 'TH', 'TW', 'UA', 'US', 'UY', 'VE', 'VI', 'VN', 'YU', 'ZA'))) {
    $format['locality_block']['administrative_area'] = array(
      '#title' => t('State'),
      '#size' => 10,
      '#required' => TRUE,
      '#prefix' => ' ',
      '#attributes' => array('class' => array('state')),
    );
  }

  // A few contries have a well-known list of administrative divisions.
  if ($address['country'] == 'US') {
    $format['locality_block']['administrative_area']['#options'] = array(
      ''   => t('--'),
      'AL' => t('Alabama'),
      'AK' => t('Alaska'),
      'AZ' => t('Arizona'),
      'AR' => t('Arkansas'),
      'CA' => t('California'),
      'CO' => t('Colorado'),
      'CT' => t('Connecticut'),
      'DE' => t('Delaware'),
      'DC' => t('District Of Columbia'),
      'FL' => t('Florida'),
      'GA' => t('Georgia'),
      'HI' => t('Hawaii'),
      'ID' => t('Idaho'),
      'IL' => t('Illinois'),
      'IN' => t('Indiana'),
      'IA' => t('Iowa'),
      'KS' => t('Kansas'),
      'KY' => t('Kentucky'),
      'LA' => t('Louisiana'),
      'ME' => t('Maine'),
      'MD' => t('Maryland'),
      'MA' => t('Massachusetts'),
      'MI' => t('Michigan'),
      'MN' => t('Minnesota'),
      'MS' => t('Mississippi'),
      'MO' => t('Missouri'),
      'MT' => t('Montana'),
      'NE' => t('Nebraska'),
      'NV' => t('Nevada'),
      'NH' => t('New Hampshire'),
      'NJ' => t('New Jersey'),
      'NM' => t('New Mexico'),
      'NY' => t('New York'),
      'NC' => t('North Carolina'),
      'ND' => t('North Dakota'),
      'OH' => t('Ohio'),
      'OK' => t('Oklahoma'),
      'OR' => t('Oregon'),
      'PA' => t('Pennsylvania'),
      'RI' => t('Rhode Island'),
      'SC' => t('South Carolina'),
      'SD' => t('South Dakota'),
      'TN' => t('Tennessee'),
      'TX' => t('Texas'),
      'UT' => t('Utah'),
      'VT' => t('Vermont'),
      'VA' => t('Virginia'),
      'WA' => t('Washington'),
      'WV' => t('West Virginia'),
      'WI' => t('Wisconsin'),
      'WY' => t('Wyoming'),
      ' ' => t('--'),
      'AS' => t('American Samoa'),
      'FM' => t('Federated States of Micronesia'),
      'GU' => t('Guam'),
      'MH' => t('Marshall Islands'),
      'MP' => t('Northern Mariana Islands'),
      'PW' => t('Palau'),
      'PR' => t('Puerto Rico'),
      'VI' => t('Virgin Islands'),
    );
    $format['locality_block']['postal_code']['#title'] = t('ZIP Code');
  }
  else if ($address['country'] == 'IT') {
    $format['locality_block']['administrative_area']['#options'] = array(
      ''   => t('--'),
      'AG' => 'Agrigento',
      'AL' => 'Alessandria',
      'AN' => 'Ancona',
      'AO' => 'Valle d\'Aosta/Vallée d\'Aoste',
      'AP' => 'Ascoli Piceno',
      'AQ' => 'L\'Aquila',
      'AR' => 'Arezzo',
      'AT' => 'Asti',
      'AV' => 'Avellino',
      'BA' => 'Bari',
      'BG' => 'Bergamo',
      'BI' => 'Biella',
      'BL' => 'Belluno',
      'BN' => 'Benevento',
      'BO' => 'Bologna',
      'BR' => 'Brindisi',
      'BS' => 'Brescia',
      'BT' => 'Barletta-Andria-Trani',
      'BZ' => 'Bolzano/Bozen',
      'CA' => 'Cagliari',
      'CB' => 'Campobasso',
      'CE' => 'Caserta',
      'CH' => 'Chieti',
      'CI' => 'Carbonia-Iglesias',
      'CL' => 'Caltanissetta',
      'CN' => 'Cuneo',
      'CO' => 'Como',
      'CR' => 'Cremona',
      'CS' => 'Cosenza',
      'CT' => 'Catania',
      'CZ' => 'Catanzaro',
      'EN' => 'Enna',
      'FC' => 'Forlì-Cesena',
      'FE' => 'Ferrara',
      'FG' => 'Foggia',
      'FI' => 'Firenze',
      'FM' => 'Fermo',
      'FR' => 'Frosinone',
      'GE' => 'Genova',
      'GO' => 'Gorizia',
      'GR' => 'Grosseto',
      'IM' => 'Imperia',
      'IS' => 'Isernia',
      'KR' => 'Crotone',
      'LC' => 'Lecco',
      'LE' => 'Lecce',
      'LI' => 'Livorno',
      'LO' => 'Lodi',
      'LT' => 'Latina',
      'LU' => 'Lucca',
      'MB' => 'Monza e Brianza',
      'MC' => 'Macerata',
      'ME' => 'Messina',
      'MI' => 'Milano',
      'MN' => 'Mantova',
      'MO' => 'Modena',
      'MS' => 'Massa-Carrara',
      'MT' => 'Matera',
      'NA' => 'Napoli',
      'NO' => 'Novara',
      'NU' => 'Nuoro',
      'OG' => 'Ogliastra',
      'OR' => 'Oristano',
      'OT' => 'Olbia-Tempio',
      'PA' => 'Palermo',
      'PC' => 'Piacenza',
      'PD' => 'Padova',
      'PE' => 'Pescara',
      'PG' => 'Perugia',
      'PI' => 'Pisa',
      'PN' => 'Pordenone',
      'PO' => 'Prato',
      'PR' => 'Parma',
      'PT' => 'Pistoia',
      'PU' => 'Pesaro e Urbino',
      'PV' => 'Pavia',
      'PZ' => 'Potenza',
      'RA' => 'Ravenna',
      'RC' => 'Reggio di Calabria',
      'RE' => 'Reggio nell\'Emilia',
      'RG' => 'Ragusa',
      'RI' => 'Rieti',
      'RM' => 'Roma',
      'RN' => 'Rimini',
      'RO' => 'Rovigo',
      'SA' => 'Salerno',
      'SI' => 'Siena',
      'SO' => 'Sondrio',
      'SP' => 'La Spezia',
      'SR' => 'Siracusa',
      'SS' => 'Sassari',
      'SV' => 'Savona',
      'TA' => 'Taranto',
      'TE' => 'Teramo',
      'TN' => 'Trento',
      'TO' => 'Torino',
      'TP' => 'Trapani',
      'TR' => 'Terni',
      'TS' => 'Trieste',
      'TV' => 'Treviso',
      'UD' => 'Udine',
      'VA' => 'Varese',
      'VB' => 'Verbano-Cusio-Ossola',
      'VC' => 'Vercelli',
      'VE' => 'Venezia',
      'VI' => 'Vicenza',
      'VR' => 'Verona',
      'VS' => 'Medio Campidano',
      'VT' => 'Viterbo',
      'VV' => 'Vibo Valentia',
    );
    $format['locality_block']['administrative_area']['#title'] = t('Province');
  }
  else if ($address['country'] == 'BR') {
    $format['locality_block']['administrative_area']['#options'] = array(
      ''   => t('--'),
      'AC' => t('Acre'),
      'AL' => t('Alagoas'),
      'AM' => t('Amazonas'),
      'AP' => t('Amapa'),
      'BA' => t('Bahia'),
      'CE' => t('Ceara'),
      'DF' => t('Distrito Federal'),
      'ES' => t('Espirito Santo'),
      'GO' => t('Goias'),
      'MA' => t('Maranhao'),
      'MG' => t('Minas Gerais'),
      'MS' => t('Mato Grosso do Sul'),
      'MT' => t('Mato Grosso'),
      'PA' => t('Para'),
      'PB' => t('Paraiba'),
      'PE' => t('Pernambuco'),
      'PI' => t('Piaui'),
      'PR' => t('Parana'),
      'RJ' => t('Rio de Janeiro'),
      'RN' => t('Rio Grande do Norte'),
      'RO' => t('Rondonia'),
      'RR' => t('Roraima'),
      'RS' => t('Rio Grande do Sul'),
      'SC' => t('Santa Catarina'),
      'SE' => t('Sergipe'),
      'SP' => t('Sao Paulo'),
      'TO' => t('Tocantins'),
    );
  }
  else if ($address['country'] == 'CA') {
    $format['locality_block']['administrative_area']['#options'] = array(
      ''   => t('--'),
      'AB' => t('Alberta'),
      'BC' => t('British Columbia'),
      'MB' => t('Manitoba'),
      'NB' => t('New Brunswick'),
      'NL' => t('Newfoundland'),
      'NT' => t('Northwest Territories'),
      'NS' => t('Nova Scotia'),
      'NU' => t('Nunavut'),
      'ON' => t('Ontario'),
      'PE' => t('Prince Edward Island'),
      'QC' => t('Quebec'),
      'SK' => t('Saskatchewan'),
      'YT' => t('Yukon Territory'),
    );
    $format['locality_block']['administrative_area']['#title'] = t('Province');
  }
  else if ($address['country'] == 'AU') {
    $format['locality_block']['administrative_area']['#options'] = array(
      ''   => t('--'),
      'ACT' => t('Australian Capital Territory'),
      'NSW' => t('New South Wales'),
      'NT' => t('Northern Territory'),
      'QLD' => t('Queensland'),
      'SA' => t('South Australia'),
      'TAS' => t('Tasmania'),
      'VIC' => t('Victoria'),
      'WA' => t('Western Australia'),
    );
  }
  
    else if ($address['country'] == 'CL') {
    $format['locality_block']['administrative_area']['#options'] = array(
''   => t('--SELECCIONE--'),
'ABO'=>t('ALGARROBO'),
'LAS'=>t('ALTO BIOBIO'),
'ADC'=>t('ALTO DEL CARMEN'),
'AHO'=>t('ALTO HOSPICIO'),
'ACD'=>t('ANCUD'),
'JAB'=>t('ANDACOLLO'),
'ZOL'=>t('ANGOL'),
'LTH'=>t('ANTARTICA'),
'ANF'=>t('ANTOFAGASTA'),
'AUO'=>t('ANTUCO'),
'ARA'=>t('ARAUCO'),
'ARI'=>t('ARICA'),
'WPA'=>t('AYSEN'),
'BUI'=>t('BUIN'),
'BLN'=>t('BULNES'),
'CDO'=>t('CABILDO'),
'LTJ'=>t('CABO DE HORNOS'),
'CRO'=>t('CABRERO'),
'CJC'=>t('CALAMA'),
'CBU'=>t('CALBUCO'),
'CLR'=>t('CALDERA'),
'LC7'=>t('CALERA'),
'ZCT'=>t('CALERA DE TANGO'),
'CLG'=>t('CALLE LARGA'),
'CLL'=>t('CANELA'),
'CRH'=>t('CARAHUE'),
'CRT'=>t('CARTAGENA'),
'ZCB'=>t('CASABLANCA'),
'CTR'=>t('CASTRO'),
'CAT'=>t('CATEMU'),
'CQE'=>t('CAUQUENES'),
'FWN'=>t('CAÑETE'),
'FCV'=>t('CERRILLOS'),
'CHA'=>t('CHANCO'),
'ZHN'=>t('CHAÑARAL'),
'ZHE'=>t('CHEPICA'),
'CYE'=>t('CHIGUAYANTE'),
'CLH'=>t('CHILE CHICO'),
'YAI'=>t('CHILLAN'),
'CHB'=>t('CHIMBARONGO'),
'ZHL'=>t('CHOLCHOL'),
'CHO'=>t('CHONCHI'),
'CBR'=>t('COBQUECURA'),
'CHM'=>t('COCHAMO'),
'CGY'=>t('CODEGUA'),
'CUH'=>t('COIHUECO'),
'CNO'=>t('COINCO'),
'CLB'=>t('COLBUN'),
'GEG'=>t('COLINA'),
'CPI'=>t('COLLIPULLI'),
'CTO'=>t('COLTAUCO'),
'COB'=>t('COMBARBALA'),
'CCP'=>t('CONCEPCION'),
'GKF'=>t('CONCON'),
'CTT'=>t('CONSTITUCION'),
'CTM'=>t('CONTULMO'),
'CPO'=>t('COPIAPO'),
'COQ'=>t('COQUIMBO'),
'KQQ'=>t('CORONEL'),
'CRC'=>t('CORRAL'),
'GXQ'=>t('COYHAIQUE'),
'CUC'=>t('CUNCO'),
'CCC'=>t('CURACAUTIN'),
'CDV'=>t('CURACO DE VELEZ'),
'CNE'=>t('CURANILAHUE'),
'CRE'=>t('CURARREHUE'),
'CEP'=>t('CUREPTO'),
'ZCQ'=>t('CURICO'),
'DLE'=>t('DALCAHUE'),
'DAG'=>t('DIEGO DE ALMAGRO'),
'RDD'=>t('DOÑIHUE'),
'GJR'=>t('EL CARMEN'),
'EMT'=>t('EL MONTE'),
'EQO'=>t('EL QUISCO'),
'ETB'=>t('EL TABO'),
'EPD'=>t('EMPEDRADO'),
'ECA'=>t('ERCILLA'),
'FCY'=>t('ESTACION CENTRAL'),
'FRD'=>t('FLORIDA'),
'FIE'=>t('FREIRE'),
'FRN'=>t('FREIRINA'),
'FSA'=>t('FRESIA'),
'FRT'=>t('FRUTILLAR'),
'FTF'=>t('FUTALEUFU'),
'FTO'=>t('FUTRONO'),
'GAO'=>t('GALVARINO'),
'GEA'=>t('GORBEA'),
'GRA'=>t('GRANEROS'),
'HJA'=>t('HIJUELAS'),
'HLE'=>t('HUALAÑE'),
'KQL'=>t('HUALQUI'),
'HRA'=>t('HUARA'),
'HCO'=>t('HUASCO'),
'FCZ'=>t('HUECHURABA'),
'ILL'=>t('ILLAPEL'),
'FDA'=>t('INDEPENDENCIA'),
'IQQ'=>t('IQUIQUE'),
'GEK'=>t('ISLA DE MAIPO'),
'IPC'=>t('ISLA DE PASCUA'),
'IJF'=>t('JUAN FERNANDEZ'),
'FDB'=>t('LA CISTERNA'),
'LCR'=>t('LA CRUZ'),
'LEA'=>t('LA ESTRELLA'),
'FDD'=>t('LA FLORIDA'),
'FDE'=>t('LA GRANJA'),
'LHI'=>t('LA HIGUERA'),
'LLC'=>t('LA LIGUA'),
'LPT'=>t('LA PINTANA'),
'FDF'=>t('LA REINA'),
'LSC'=>t('LA SERENA'),
'ZLU'=>t('LA UNION'),
'LRC'=>t('LAGO RANCO'),
'LVE'=>t('LAGO VERDE'),
'LSG'=>t('LAMPA'),
'LNC'=>t('LANCO'),
'LCB'=>t('LAS CABRAS'),
'FDG'=>t('LAS CONDES'),
'LTR'=>t('LAUTARO'),
'ZLB'=>t('LEBU'),
'LNN'=>t('LICANTEN'),
'LIC'=>t('LIMACHE'),
'ZLR'=>t('LINARES'),
'LEC'=>t('LITUECHE'),
'GRK'=>t('LLAILLAY'),
'LLQ'=>t('LLANQUIHUE'),
'FDH'=>t('LO BARNECHEA'),
'LLL'=>t('LOLOL'),
'LOC'=>t('LONCOCHE'),
'LGV'=>t('LONGAVI'),
'LQY'=>t('LONQUIMAY'),
'ZAO'=>t('LOS ALAMOS'),
'LAC'=>t('LOS ANDES'),
'LSQ'=>t('LOS ANGELES'),
'LAG'=>t('LOS LAGOS'),
'LMM'=>t('LOS MUERMOS'),
'SUS'=>t('LOS SAUCES'),
'LV7'=>t('LOS VILOS'),
'LOT'=>t('LOTA'),
'LMO'=>t('LUMACO'),
'MCH'=>t('MACHALI'),
'FDJ'=>t('MACUL'),
'MFL'=>t('MAFIL'),
'FDK'=>t('MAIPU'),
'ZML'=>t('MALLOA'),
'MCG'=>t('MARCHIHUE'),
'MAE'=>t('MARIA ELENA'),
'MAO'=>t('MARIA PINTO'),
'MQA'=>t('MARIQUINA'),
'ZMA'=>t('MAULE'),
'MAU'=>t('MAULLIN'),
'MJS'=>t('MEJILLONES'),
'MPO'=>t('MELIPEUCO'),
'MPA'=>t('MELIPILLA'),
'ZMO'=>t('MOLINA'),
'MPC'=>t('MONTE PATRIA'),
'MUL'=>t('MULCHEN'),
'NAC'=>t('NACIMIENTO'),
'ZGA'=>t('NANCAGUA'),
'PNT'=>t('NATALES'),
'NVD'=>t('NAVIDAD'),
'NRE'=>t('NEGRETE'),
'NNH'=>t('NINHUE'),
'NOG'=>t('NOGALES'),
'NIP'=>t('NUEVA IMPERIAL'),
'OLM'=>t('OLMUE'),
'ZOS'=>t('OSORNO'),
'OVL'=>t('OVALLE'),
'PHT'=>t('PADRE HURTADO'),
'PIO'=>t('PAIGUANO'),
'PAI'=>t('PAILLACO'),
'GEJ'=>t('PAINE'),
'PLA'=>t('PALMILLA'),
'PGP'=>t('PANGUIPULLI'),
'PQE'=>t('PANQUEHUE'),
'PPO'=>t('PAPUDO'),
'PDS'=>t('PAREDONES'),
'PRR'=>t('PARRAL'),
'PLO'=>t('PELARCO'),
'PMO'=>t('PEMUCO'),
'PNH'=>t('PENCAHUE'),
'PQO'=>t('PERQUENCO'),
'PTK'=>t('PETORCA'),
'PEO'=>t('PEUMO'),
'GSI'=>t('PEÑAFLOR'),
'FDY'=>t('PEÑALOLEN'),
'OPC'=>t('PICA'),
'PDG'=>t('PICHIDEGUA'),
'PMU'=>t('PICHILEMU'),
'PNO'=>t('PINTO'),
'PIR'=>t('PIRQUE'),
'PQN'=>t('PITRUFQUEN'),
'PZO'=>t('PORTEZUELO'),
'ZPR'=>t('PORVENIR'),
'PAM'=>t('POZO ALMONTE'),
'FDN'=>t('PROVIDENCIA'),
'PCV'=>t('PUCHUNCAVI'),
'ZPU'=>t('PUCON'),
'FDO'=>t('PUDAHUEL'),
'PTA'=>t('PUENTE ALTO'),
'PMC'=>t('PUERTO MONTT'),
'PCY'=>t('PUERTO OCTAY'),
'ZPV'=>t('PUERTO VARAS'),
'PNQ'=>t('PUMANQUE'),
'PTQ'=>t('PUNITAQUI'),
'PUQ'=>t('PUNTA ARENAS'),
'PQU'=>t('PUQUELDON'),
'PUR'=>t('PUREN'),
'PRE'=>t('PURRANQUE'),
'PUT'=>t('PUTAENDO'),
'PTR'=>t('PUTRE'),
'QIL'=>t('QUEILEN'),
'QLO'=>t('QUELLON'),
'QMC'=>t('QUEMCHI'),
'QCO'=>t('QUILACO'),
'QRA'=>t('QUILICURA'),
'QEO'=>t('QUILLECO'),
'QLL'=>t('QUILLON'),
'QTA'=>t('QUILLOTA'),
'QPE'=>t('QUILPUE'),
'QCC'=>t('QUINTA DE TILCOCO'),
'FDP'=>t('QUINTA NORMAL'),
'QTO'=>t('QUINTERO'),
'QRH'=>t('QUIRIHUE'),
'QRC'=>t('RANCAGUA'),
'RAU'=>t('RAUCO'),
'FDQ'=>t('RECOLETA'),
'RNA'=>t('RENAICO'),
'RGO'=>t('RENGO'),
'REQ'=>t('REQUINOA'),
'RTO'=>t('RETIRO'),
'RBN'=>t('RIO BUENO'),
'RRO'=>t('RIO CLARO'),
'RNC'=>t('RIO NEGRO'),
'RML'=>t('ROMERAL'),
'SAF'=>t('SAGRADA FAMILIA'),
'SCA'=>t('SALAMANCA'),
'QTN'=>t('SAN ANTONIO'),
'SBD'=>t('SAN BERNARDO'),
'SCS'=>t('SAN CARLOS'),
'SCQ'=>t('SAN CLEMENTE'),
'ZEB'=>t('SAN ESTEBAN'),
'SFP'=>t('SAN FELIPE'),
'SF7'=>t('SAN FERNANDO'),
'IUT'=>t('SAN GREGORIO'),
'GIY'=>t('SAN IGNACIO'),
'SJA'=>t('SAN JAVIER'),
'SJS'=>t('SAN JOSE DE MAIPO'),
'SJC'=>t('SAN JUAN DE LA COSTA'),
'FDS'=>t('SAN MIGUEL'),
'SNL'=>t('SAN NICOLAS'),
'SPL'=>t('SAN PABLO'),
'ZPM'=>t('SAN PEDRO'),
'SPX'=>t('SAN PEDRO DE ATACAMA'),
'SPZ'=>t('SAN PEDRO DE LA PAZ'),
'GIP'=>t('SAN RAFAEL'),
'SRO'=>t('SAN ROSENDO'),
'SVT'=>t('SAN VICENTE'),
'SBB'=>t('SANTA BARBARA'),
'ZSC'=>t('SANTA CRUZ'),
'SAJ'=>t('SANTA JUANA'),
'SCL'=>t('SANTIAGO'),
'SGD'=>t('SIERRA GORDA'),
'TNT'=>t('TALAGANTE'),
'TLX'=>t('TALCA'),
'ZTO'=>t('TALCAHUANO'),
'GTE'=>t('TALTAL'),
'ZCO'=>t('TEMUCO'),
'TEN'=>t('TENO'),
'TSC'=>t('TEODORO SCHMIDT'),
'TRM'=>t('TIERRA AMARILLA'),
'EXI'=>t('TILTIL'),
'TRU'=>t('TIRUA'),
'TOC'=>t('TOCOPILLA'),
'TMC'=>t('TOME'),
'ZEN'=>t('TRAIGUEN'),
'THC'=>t('TREGUACO'),
'TAL'=>t('TUCAPEL'),
'ZAL'=>t('VALDIVIA'),
'VLR'=>t('VALLENAR'),
'VAP'=>t('VALPARAISO'),
'VCH'=>t('VICHUQUEN'),
'VIC'=>t('VICTORIA'),
'VCA'=>t('VICUÑA'),
'VLN'=>t('VILCUN'),
'VGE'=>t('VILLA ALEGRE'),
'VIA'=>t('VILLA ALEMANA'),
'VRR'=>t('VILLARRICA'),
'FDW'=>t('VITACURA'),
'KNA'=>t('VIÑA DEL MAR'),
'YBB'=>t('YERBAS BUENAS'),
'YGY'=>t('YUNGAY'),
'JJG'=>t('ZAPALLAR'),
'NQN'=>t('ÑIQUEN'),
'FDL'=>t('ÑUÑOA'),
    );
    
    // Administrative
    $format['locality_block']['administrative_area'] = array_merge(
      $format['locality_block']['administrative_area'],
      array(
        '#title' => t('Comuna'),
        '#required' => TRUE
      )
    );

    // Postal Code
    $format['locality_block']['postal_code'] = array_merge(
      $format['locality_block']['postal_code'],
      array(
        '#title' => t('Código Postal'),
        '#required' => FALSE
      )
    );

    $format['locality_block']['locality']['#title'] = t('Ciudad');

    $format['street_block']['thoroughfare']['#title'] = t('Dirección Principal');
    $format['street_block']['premise']['#title'] = t('Dirección Secundaria');


  }

  // Those countries tend to put the postal code after the locality.
  if (in_array($address['country'], array('AU', 'BD', 'BF', 'BH', 'BM', 'BN', 'BT', 'CA', 'FM', 'GB', 'ID', 'IN', 'JM', 'JO', 'KH', 'LB', 'LS', 'LV', 'MM', 'MN', 'MV', 'MW', 'NG', 'NP', 'NZ', 'PE', 'PK', 'PR', 'PW', 'SA', 'SG', 'SO', 'TH', 'US', 'VI', 'VG', 'VN'))) {
    // Take the widget out of the array.
    $postal_code_widget = $format['locality_block']['postal_code'];
    $postal_code_widget['#prefix'] = ' ';
    unset($format['locality_block']['postal_code']);

    // Add it back.
    $format['locality_block']['postal_code'] = $postal_code_widget;

    // Remove the prefix from the first widget of the block.
    $element_children = element_children($format['locality_block']);
    $first_child = reset($element_children);
    unset($format['locality_block'][$first_child]['#prefix']);
  }

  // GB-specific tweaks
  if ($address['country'] == 'GB') {
    // Locality
    $format['locality_block']['locality'] = array_merge(
      $format['locality_block']['locality'],
      array(
        '#title' => t('Town/City'),
        '#weight' => 40,
        '#prefix' => '',
        '#tag' => 'div',
      )
    );

    // Administrative
    $format['locality_block']['administrative_area'] = array_merge(
      $format['locality_block']['administrative_area'],
      array(
        '#title' => t('County'),
        '#required' => FALSE,
        '#weight' => 50,
        '#size' => 30,
        '#prefix' => '',
        '#tag' => 'div',
      )
    );

    // Postal code
    $format['locality_block']['postal_code'] = array_merge(
      $format['locality_block']['postal_code'],
      array(
        '#title' => t('Postcode'),
        '#weight' => 60,
        '#prefix' => '',
        '#tag' => 'div',
      )
    );
  }

  if ($context['mode'] == 'form') {
    // Provide a wrapper ID for AJAX replacement based on country selection.
    if (!isset($format['#wrapper_id'])) {
      $format['#wrapper_id'] = drupal_html_id('addressfield-wrapper');
      $format['#prefix'] = '<div id="' . $format['#wrapper_id'] . '">';
      $format['#suffix'] = '</div>';
    }

    // Form mode, move the country selector to the top of the form.
    $format['country']['#weight'] = -10;

    // Limit it to the countries supported by the widget.
    if (isset($context['field'])) {
      $format['country']['#options'] = _addressfield_country_options_list($context['field'], $context['instance']);
    }

    // AJAX enable it.
    $format['country']['#ajax'] = array(
      'callback' => 'addressfield_standard_widget_refresh',
      'wrapper' => $format['#wrapper_id'],
      'method' => 'replace',
    );
    $format['country']['#element_validate'] = array('addressfield_standard_country_validate');
    // Don't validate any element when the country is changed.
    $format['country']['#limit_validation_errors'] = array();

    if (isset($context['delta']) && $context['delta'] > 0) {
      // On subsequent elements of a field, we make the country field non
      // required and add a ' - None - ' option to it, so as to allow the
      // user to remove the address by clearing the country field.
      $format['country']['#required'] = FALSE;
      $format['country']['#empty_value'] = '';
    }
  }
}
