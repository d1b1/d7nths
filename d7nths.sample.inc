<?php
/*
 * This is a set of sample functions you can use to make your 
 * install function create a new content type. Use the export to
 * define the values for the content type, fields and instances.
 *
 * Contains:
 *
 *   1. Sample Install function.
 *   2. Sample Content Type Definition function.
 *   3. Sample Fields definition function.
 *   4. Sample Instances defintion function.
 *
 */
 
/*
 * This implements the hook_install function.
 *
 * To Use: Change the function name to your module.
 *
 */
function newmodule_install() {

  // get the translation function relevant to our current localisation
  $t = get_t();

  $new_content_type = _newcontent_type();
  
  $content_type = node_type_set_defaults( $new_content_type );

  // Add the Body to the Content Type
  node_add_body_field($content_type, 'Body');

  // Now Create the content type
  node_type_save($content_type);

  // First we create the fields.
  foreach (_newcontent_installed_fields() as $field) {
    if (!field_info_field($field['field_name'])) {
      field_create_field($field);
    }
  }

  // Second we create the field instances for our fields.
  foreach (_newcontent_installed_instances() as $instance) {
    if (!field_info_instance($instance['entity_type'], $instance['field_name'], $instance['bundle'])) {
      field_create_instance($instance);
    }
  }
  
}

/*
 * This implements the hook_ininstall function.
 *
 * To Use: Change the function name to your module.
 *
 */
function newmodule_ininstall() {

  // Gather all the program content that might have been created 
  // while this module was enabled. Simple selects still use db_query().

  $new_content_type = _newcontent_type();
  
  $sql = 'SELECT nid FROM {node} n WHERE n.type = :type';
  $result = db_query($sql, array(':type' => $new_content_type['type']));
  $nids = array();
  foreach ($result as $row) {
    $nids[] = $row->nid;
  }

  // Delete all the nodes at once
  node_delete_multiple($nids);

  // Loop over each of the fields defined by this module and delete
  // all instances of the field, their data, and the field itself.
  foreach (array_keys(_song_installed_fields()) as $field) {
    if (field_info_field($field['field_name'])) {
      field_delete_field($field);
    }
  }

  // Loop over any remaining field instances attached to the node_example
  $instances = field_info_instances('node', $new_content_type['type']);
  foreach ($instances as $instance_name => $instance) {
    if (field_info_instance($instance['entity_type'], $instance['field_name'], $instance['bundle'])) {
      field_delete_instance($instance);
    }
  }

  // Delete our content type
  node_type_delete($new_content_type['type']);

  // Purge all field infromation
  field_purge_batch(1000);
  
}

/*
 * Sample Function for your Install File. This will 
 * Define the array of new content types you want to use.
 *
 * To Use: Rename as ModuleName_content_types()
 *
 */
function _newcontent_type() {

  /*
    $content_types = array(
      'contenttype' = array(
        'type'        => 'newcontent',
        'name'        => $t('New Content'),
        'base'        => 'node_content',
        'description' => $t('This provdies the ability to create new content.'),
        'body_label'  => $t('Description'),
        'promote'     => 0,
        'status'      => 1,
        'comment'     => 0,
       )
    );
    
    // Use the export to create the content type definition.
  
  */
  
  return $content_types;
}

/*
 * Sample Function for your Array of Fields. This will 
 * Define the array of new content types you want to use.
 *
 */
function _newcontent_installed_fields() {

  $fields = array();
  
  //
  // Paste in the content from the export.
  //
  
  return $fields;
}

/*
 * Sample Function for your Array of Fields. This will 
 * Define the array of new content types you want to use.
 *
 */
function _newcontent_installed_instances() {

  $instances = array();
  
  //
  // Paste in the content from the export.
  //
  
  return $instances;
}