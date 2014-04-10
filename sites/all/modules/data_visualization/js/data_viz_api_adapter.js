var DATAVIZ = { };

(function($){

DATAVIZ.adapters = new Array();

DATAVIZ.adapters['jquery.jqplot'] = {
  render : function(data_object, type, container_id) {
    $.getScript('/' + Drupal.settings.data_visualization.data_visualization_module_path + '/adapters/DATAVIZ.jqplot.js', function(data, textStatus){
      DATAVIZ.jqplot.render(data_object, type, container_id);
      return true;
    });
  }
};

DATAVIZ.adapters['jquery.jit'] = {
  render : function(data_object, type, container_id) {
    $.getScript('/' + Drupal.settings.data_visualization.data_visualization_module_path + '/adapters/DATAVIZ.jit.js', function(data, textStatus){
      DATAVIZ.jit.render(data_object, type, container_id);
      return true;
    });
  }
};

DATAVIZ.adapters['jquery.highcharts'] = {
  render : function(data_object, type, container_id){
    $.getScript('/' + Drupal.settings.data_visualization.data_visualization_module_path + '/adapters/DATAVIZ.highcharts.js', function(data, textStatus){
      DATAVIZ.highcharts.render(data_object, type, container_id);
      return true;
    });
  }
};

DATAVIZ.render = function(adapter, data_object, type, container_id) {
  //Load the obect whether from file or passed into the function
  if (data_object.constructor == String) {
    var filepath = data_object;
    $.getJSON(filepath, function(data){
      //Call the render function of the passed in adapter object
      adapter.render(data, type, container_id);
    });
  } else {
    //Call the render function of the passed in adapter object
    adapter.render(data_object, type, container_id);
  }  
};

})(jQuery);
