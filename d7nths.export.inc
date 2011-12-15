<?php

/*
 * This function builds the export form for content type fields.
 *
 */
function d7nths_export_form($form, &$form_state) {

  $form = array();

  $options = array();
  $types = node_type_get_types();
  foreach ($types as $name => $type) {
    $options[$name] = $type->name;
  }

  $form['content_types'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('Choose a content type'),
    '#description'   => 'Users always receive the <em>authenticated user</em> role.',
    '#options'       => $options,
    '#default_value' => isset($_SESSION['d7nth_content_types']) ? $_SESSION['d7nth_content_types'] : '',
  );
  
  $form['data_options'] = array(
    '#type'          => 'checkboxes',
    '#title'         => t('Give me'),
    '#description'   => 'This provides the ability to choose what parts of the content type you want to export.',
    '#options'       => array(
      'fields' => 'fields',
      'instances' => 'instances'
    ),
    '#default_value' => isset($_SESSION['data_options']) ? $_SESSION['data_options'] : array('instances'),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => 'Submit',
  );

  $as_functions = FALSE;
  
  if (isset($_SESSION['d7nth_content_types'])) {
    include_once DRUPAL_ROOT . '/includes/utility.inc';

    $dump_fields = array();
    $dump_instances = array();

    foreach($_SESSION['d7nth_content_types'] as $bundle => $bundle_name) {

      if ($bundle_name) {
        // TODO - Need to understand the EntityType a little better.
        $entity_type = 'node';
        
        foreach (field_info_instances('node', $bundle_name) as $field_name => $field) {
        
          // Note: Borrowed this code from a post on now to export ccs.
          // URL: http://steindom.com/articles/exporting-and-creating-field-definitions-drupal-7
          // Thanks: Steindom, LLC
          
          $info_config = field_info_field($field_name);
          $info_instance = field_info_instance($entity_type, $field_name, $bundle_name);
          
          unset($info_config['id']);
          unset($info_instance['id'], $info_instance['field_id']);
        
          if ($as_functions) {
            $output .= "field_create_field(" . drupal_var_export( $info_config ) . ");\n";
            $output .= "field_create_instance(" . drupal_var_export( $info_instance ) . ");";
          } else {
            $dump_fields[$info_config['field_name']] = $info_config; 
            $dump_instances[$info_config['field_name']] = $info_instance;
          }
          
        }  // EOI
        
        // Define the fieldset.
        
        $form['fieldset-' . $bundle_name] = array(
          '#type'        => 'fieldset',
          '#value'       => $output,
          '#title'       => t('Content Type: ' . $bundle_name),
          '#description' => 'The following are the fields and instances for the a content type.',
        );
          
        if ($_SESSION['data_options']['fields']) {
        
          $output = "// Fields\n";
          $output .= '$' . "fields = " . drupal_var_export( $dump_fields );
        
          $form['fieldset-' . $bundle_name]['dump-fields-' . $bundle_name] = array(
            '#type'        => 'textarea',
            '#value'       => $output . ';',
            '#title'       => t('Exported of Fields for ' . $bundle_name),
            '#description' => 'Copy the content of this textarea and use in your module install function.',
            '#rows'        => 10,
          );
        }
        
        if ($_SESSION['data_options']['instances']) {
        
          $output = "// Instances\n";
          $output .= '$' . "instances = " . drupal_var_export( $dump_instances );
        
          $form['fieldset-' . $bundle_name]['dump-instances-' . $bundle_name] = array(
            '#type'        => 'textarea',
            '#value'       => $output . ';',
            '#title'       => t('Exported of Instances for ' . $bundle_name),
            '#description' => 'Copy the content of this textarea and use in your module install function.',
            '#rows'        => 10,
          );
        }
      
      } // EOL
 
    }
  }
  
  return $form;
}

/*
 * Hacky solution, this is the submit function for the export form.
 * It dumps the content types into session and redirects back to the form.
 * And done.
 *
 */
function d7nths_export_form_submit($form, &$form_state) {

  $_SESSION['d7nth_content_types'] = $form_state['values']['content_types'];
  $_SESSION['data_options'] = $form_state['values']['data_options'];

  drupal_goto('admin/structure/types/export');
}
