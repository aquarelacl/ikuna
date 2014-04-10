<?php

/**
 * @file
 * This file contains the function that adds necessary library javascrip to the
 * page given a library input parameter.
 */

/**
 * data_visualization_libraries_load
 */
function data_visualization_libraries_load($library) {
  $library_path = libraries_get_path($library);

  switch ($library) {
    case 'jquery.jqplot':
      drupal_add_js($library_path . '/jquery.jqplot.js', 'file');
      drupal_add_css($library_path . '/jquery.jqplot.min.css', 'file');
      drupal_add_js($library_path . '/plugins/jqplot.barRenderer.min.js', 'file');
      drupal_add_js($library_path . '/plugins/jqplot.categoryAxisRenderer.min.js', 'file');
      drupal_add_js($library_path . '/plugins/jqplot.pointLabels.min.js', 'file');
      drupal_add_js($library_path . '/plugins/jqplot.pieRenderer.min.js', 'file');
      drupal_add_js($library_path . '/plugins/jqplot.donutRenderer.min.js', 'file');
      break;

    case 'jquery.jit':
      drupal_add_js($library_path . '/jit.js', 'file');
      drupal_add_js($library_path . '/jit-yc.js', 'file');
      break;

    case 'jquery.highcharts':
      drupal_add_js($library_path . '/js/highcharts.js', 'file');
      break;
  }
}
