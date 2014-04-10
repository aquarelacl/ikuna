<?php

/**
 * @file
 * Theme function to render container objects.
 * Not yet used.
 */

/**
 * theme_data_visualization_adapter
 *
 */
function theme_data_visualization_adapter() {
  $data = '';
  $data .= '<div id="jqplot-line" style="width:500px;height:300px;"></div>';
  $data .= '<div id="jqplot-bar" style="width:500px;height:300px;"></div>';
  $data .= '<div id="jqplot-doe" style="width:700px;height:500px;"></div>';

  $data .= '<div id="jit-line" style="width:500px;height:300px;"></div>';
  $data .= '<div id="jit-bar" style="width:500px;height:300px;"></div>';
  $data .= '<div id="jit-doe" style="width:700px;height:500px;"></div>';

  $data .= '<div id="highcharts-line" style="width:700px;height:300px;"></div>';
  $data .= '<div id="highcharts-bar" style="width:700px;height:300px;"></div>';
  $data .= '<div id="highcharts-doe" style="width:700px;height:500px;"></div>';

  return $data;
}
