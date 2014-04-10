<?php

/**
 * @file
 * This file containts the DataVisualizationController. Has definitions of the
 * save and delete methods to save and delete data_visualization entities.
 */

/**
 * Extend entity controller for data_visualization
 *
 */
class DataVisualizationController extends DrupalDefaultEntityController {

  /**
   * Save function for DataVisualizationController
   */
  public function save($data_visualization) {
    if (!empty($data_visualization->dvid)) {
      $data_visualization->updated = time();
      drupal_write_record('data_visualization', $data_visualization, array('dvid'));
    }
    else {
      $data_visualization->created = time();
      drupal_write_record('data_visualization', $data_visualization);
    }
    field_attach_insert('data_visualization', $data_visualization);
    module_invoke_all('entity_insert', 'data_visualization', $data_visualization);

    return $data_visualization;
  }

  /**
   * Delete function for DataVisualizationController
   */
  public function delete($data_visualization) {
    // Delete the associated data file.
    $file = file_load($data_visualization->fid);
    file_delete($file);
    $data_visualization_title = $data_visualization->title;

    // Delete the entity record.
    $num_deleted = db_delete('data_visualization')
      ->condition('dvid', $data_visualization->dvid)
      ->execute();

    return $num_deleted;
  }
}
