<?php

/**
 * @file
 * This file contains the form for editing/creating data_visualization
 * entities. Also contains the validation and submit functions for the form.
 */

/**
 * data_visualization_edit_form
 *
 */
function data_visualization_edit_form($form, &$form_state, $data_visualization) {
  // Allowed libraries go into the $allowed_libraries array.
  // TODO: Make this a config variable.
  $allowed_libraries = array('jquery.jqplot', 'jquery.jit', 'jquery.highcharts');

  // For file attachments.
  $form = array('#attributes' => array('enctype' => "multipart/form-data"));

  $form['dvid'] = array(
    '#type' => 'value',
    '#value' => $data_visualization->dvid,
  );

  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Data Visualization Title'),
    '#required' => TRUE,
    '#default_value' => $data_visualization->title,
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Data Visualization Description'),
    '#required' => FALSE,
    '#default_value' => $data_visualization->description,
  );

  $form['data_file'] = array(
    '#type' => 'managed_file',
    '#title' => t('Data File'),
    '#size' => 48,
    '#description' => t('Upload a json file written in !Vizstruct-link json notation.', array('!Vistruct-link', l('Vizstruct', drupal_get_path('module', 'data_visualization_api') . '/spec.md', array('attributes' => array('target' => '_blank'))))),
    '#upload_location' => 'public://data-visualization-files',
    '#default_value' => $data_visualization->fid,
  );

  $form['viz_type'] = array(
    '#type' => 'radios',
    '#title' => t('Visualization Type'),
    '#default_value' => $data_visualization->viz_type,
    '#options' => array('line' => t('Line'), 'bar' => t('Bar'), 'pie' => t('Pie')),
  );

  $libraries = libraries_get_libraries();

  $libraries_options = array();
  foreach ($libraries as $library_name => $library_path) {
    if (in_array($library_name, array_values($allowed_libraries))) {
      $libraries_options[$library_name] = $library_name;
    }
  }

  $form['library'] = array(
    '#type' => 'radios',
    '#title' => t('Visualization Library'),
    '#default_value' => $data_visualization->library,
    '#options' => $libraries_options,
  );

  // Load the data file and load some default options.
  $file = file_load($data_visualization->fid);
  $data_file_contents = file_get_contents($file->uri);

  $data_object_from_file = json_decode($data_file_contents);

  $form['chart_options'] = array(
    '#type' => 'fieldset',
    '#title' => t('Chart Options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['chart_options']['legend'] = array(
    '#type' => 'fieldset',
    '#title' => t('Legend'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['chart_options']['legend']['show_legend'] = array(
    '#type' => 'radios',
    '#title' => t('Show Legend'),
    '#options' => array(0 => t('Hide'), 1 => t('Show')),
    '#default_value' => (!empty($data_object_from_file->options->legend)) ? (int) $data_object_from_file->options->legend->show : 0,
  );

  $form['chart_options']['legend']['legend_location'] = array(
    '#type' => 'radios',
    '#title' => t('Legend Position'),
    '#options' => array('n' => t('North'), 's' => t('South'), 'e' => t('East'), 'w' => t('West'), 'ne' => t('Northeast'), 'nw' => t('Northwest'), 'se' => t('Southeast'), 'sw' => t('Southwest')),
    '#default_value' => (!empty($data_object_from_file->options->legend)) ? $data_object_from_file->options->legend->location : 'e',
  );

  $form['chart_options']['colors'] = array(
    '#type' => 'fieldset',
    '#title' => t('Colors'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  foreach ($data_object_from_file->series as $series_index => $series_object) {
    $clean_name = drupal_html_id($series_object->seriesName);
    $series_options = (array) $data_object_from_file->options->seriesOptions[$series_index]->seriesOptions;

    $form['chart_options']['colors'][$clean_name . '-color'] = array(
      '#type' => 'textfield',
      '#title' => t('Color for @series_name', array('@series_name' => $series_object->seriesName)),
      '#default_value' => (!empty($series_options['color'])) ? $series_options['color'] : '#000',
      '#description' => t('Pick a color'),
      '#size' => 9,
      '#maxlength' => 7,
      '#attached' => array(
        // Add Farbtastic color picker.
        'library' => array(
          array('system', 'farbtastic'),
        ),
      ),
      '#suffix' => "<div id='" . $clean_name . "_colorpicker'></div><script type='text/javascript'>
      jQuery(document).ready(function() {
        jQuery('#" . $clean_name . "_colorpicker').farbtastic('#edit-" . str_replace('_', '-', $clean_name) . "-color');
      });
      </script>",
    );
  }
  $color_palette = $data_object_from_file->options->colors;
  $formatted_string = '';

  foreach ($color_palette as $hex_color) {
    $formatted_string .= $hex_color . "\n";
  }

  $form['chart_options']['colors']['color_palette'] = array(
    '#type' => 'textarea',
    '#title' => t('Color Palette'),
    '#description' => t("This is for pie charts where there is one series. They are default colors for the chart's individual items. When all colors are used, new colors are pulled from the start again."),
    '#default_value' => $formatted_string,
  );

  $form['chart_options']['axes'] = array(
    '#type' => 'fieldset',
    '#title' => t('Axes'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['chart_options']['axes']['x_axis'] = array(
    '#type' => 'fieldset',
    '#title' => t('X-Axis'),
  );
  $x_axis_title = '';
  $y_axis_title = '';
  $x_axis_tickinterval = 1;

  if (!empty($data_object_from_file->options->axes)) {
    if (!empty($data_object_from_file->options->axes->xaxis)) {
      if (!empty($data_object_from_file->options->axes->xaxis->title)) {
        $x_axis_title = $data_object_from_file->options->axes->xaxis->title;
      }
      if (!empty($data_object_from_file->options->axes->xaxis->tickinterval)) {
        $x_axis_tickinterval = $data_object_from_file->options->axes->xaxis->tickinterval;
      }
    }
    if (!empty($data_object_from_file->options->axes->yaxis)) {
      if ($data_object_from_file->options->axes->yaxis->title) {
        $y_axis_title = $data_object_from_file->options->axes->yaxis->title;
      }
    }

  }

  $form['chart_options']['axes']['x_axis']['x_axis_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $x_axis_title,
  );

  $form['chart_options']['axes']['x_axis']['x_axis_tick_interval'] = array(
    '#type' => 'textfield',
    '#title' => t('X-Axis Tick Interval'),
    '#default_value' => $x_axis_tickinterval,
  );

  $form['chart_options']['axes']['y_axis'] = array(
    '#type' => 'fieldset',
    '#title' => t('Y-Axis'),
  );

  $form['chart_options']['axes']['y_axis']['y_axis_title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#default_value' => $y_axis_title,
  );

  $form['chart_options']['plot'] = array(
    '#type' => 'fieldset',
    '#title' => t('Plot Area'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );


  $form['chart_options']['plot']['plot_background_color'] = array(
    '#type' => 'textfield',
    '#title' => t('Plot Area Background'),
    '#default_value' => '#FFF',
    '#description' => 'Pick a color for Plot background',
    '#size' => 9,
    '#maxlength' => 7,
    '#attached' => array(
      // Add Farbtastic color picker.
      'library' => array(
        array('system', 'farbtastic'),
      ),
    ),
    '#suffix' => "<div id='plot_background_colorpicker'></div><script type='text/javascript'>
      jQuery(document).ready(function() {
      jQuery('#plot_background_colorpicker').farbtastic('#edit-plot-background-color');
      });</script>",
  );

  $form['chart_options']['tooltip'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tool Tip'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['chart_options']['tooltip']['show_tooltip'] = array(
    '#type' => 'radios',
    '#title' => t('Show Tooltips'),
    '#options' => array(0 => t('Hide'), 1 => t('Show')),
    '#default_value' => ($data_object_from_file->options->tooltip->enabled) ? 1 : 0,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );

  field_attach_form('data_visualization', $data_visualization, $form, $form_state);

  return $form;
}


/**
 * data_visualization_edit_form_validate
 *
 */
function data_visualization_edit_form_validate($form, &$form_state) {
  // Make sure the x tick interval is numeric.
  if (!preg_match('/^\d+$/', $form_state['values']['x_axis_tick_interval'])) {
    form_set_error('x_axis_tick_interval', t('The X-axis tick inteval must be a number.'));
  }
}

/**
 * data_visualization_edit_form_submit
 *
 */
function data_visualization_edit_form_submit($form, &$form_state) {
  $data_visualization_submission = (object) $form_state['values'];
  field_attach_submit('data_visualization', $data_visualization_submission, $form, $form_state);

  // Save the file if the person has replaced it with a new one.
  $file = file_load($data_visualization_submission->data_file);
  $file->status = FILE_STATUS_PERMANENT;
  file_save($file);
  file_usage_add($file, 'data_visualization', 'data_visualization', $data_visualization_submission->dvid);

  $data_visualization_submission->fid = $file->fid;
  $data_visualization = data_visualization_save($data_visualization_submission);
  $form_state['redirect'] = 'data-visualization/' . $data_visualization->dvid;

  // Load the object from file and prepare to append it.
  $data_file_contents = file_get_contents($file->uri);
  $data_object_from_file = json_decode($data_file_contents);

  // The code for Title.
  $data_object_from_file->options->title->text = $data_visualization_submission->title;

  // The code for Description.
  if ($data_object_from_file->options->description && $data_visualization_submission->description) {
    $data_object_from_file->options->description->text = $data_visualization_submission->description;
  }

  // Write the series options.
  foreach ($data_object_from_file->series as $series_index => $series_object) {
    // This is for Color.
    if (!empty($data_object_from_file->options->seriesOptions[$series_index])) {
      $data_object_from_file->options->seriesOptions[$series_index] = new stdClass();
    }
    $data_object_from_file->options->seriesOptions[$series_index]->seriesOptions->color = (string) $form_state['values'][drupal_html_id($series_object->seriesName) . '-color'];
  }

  // Set the default color palette for the object.
  if (!empty($data_visualization_submission->color_palette)) {
    $colors_array = explode("\n", $data_visualization_submission->color_palette);
    $data_object_from_file->options->colors = $colors_array;
  }

  // This is for Plot Options.
  $data_object_from_file->options->plotOptions->backgroundColor = $data_visualization_submission->plot_background_color;

  // This is for Legend.
  $data_object_from_file->options->legend->show = ($data_visualization_submission->show_legend == 1) ? TRUE : FALSE;
  $data_object_from_file->options->legend->location = $data_visualization_submission->legend_location;

  // This is for X-Axis.
  if (!empty($data_visualization_submission->x_axis_title)) {
    $data_object_from_file->options->axes->xaxis->title = $data_visualization_submission->x_axis_title;
  }

  // This is for X-Axis tick interval.
  if (!empty($data_visualization_submission->x_axis_tick_interval)) {
    $data_object_from_file->options->axes->xaxis->tickinterval = $data_visualization_submission->x_axis_tick_interval;
  }

  // This is for Y-Axis.
  if (!empty($data_visualization_submission->y_axis_title)) {
    $data_object_from_file->options->axes->yaxis->title = $data_visualization_submission->y_axis_title;
  }

  // This is for Tooltip.
  $data_object_from_file->options->tooltip->enabled = ($data_visualization_submission->show_tooltip == 1) ? TRUE : FALSE;

  file_save_data(json_encode($data_object_from_file), $file->uri, FILE_EXISTS_REPLACE);
}
