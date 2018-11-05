import React, { Component } from 'react';
import PropTypes from 'prop-types';

import { Field, Form } from 'formik';
import cx from 'classnames';
import { withStyles } from '@material-ui/core/styles';
import { Grid,Tooltip,IconButton, Typography, Button } from '@material-ui/core';
import { Delete,Edit } from '@material-ui/icons';

import styles from './styles';
import TextField from 'components/inputs/TextField';
import Select from 'components/inputs/Select';
import Switch from 'components/inputs/Switch';

class Task extends Component {
  
  render() {
    //console.log(this.props);
    const { label,deleteTask,type,classes,categories, subcategories,isSubmitting,resetForm,initialValues,errors,editCategories } = this.props;
    return (
      <Form>
        <Grid container spacing={24} className={classes.gridContainer}>
          <Grid item xs={12}  className={cx(classes.headerRow,classes.row)} >
            <Typography variant="h6">{label}</Typography>
            {type === 'edit' && (
              <Tooltip title="Delete">
                <IconButton onClick={deleteTask} aria-label="Delete">
                  <Delete />
                </IconButton>
              </Tooltip>
            )}
          </Grid>
          <Grid item xs={12}  className={classes.row}>
            <Field
              name="name"
              component={TextField}
              margin="none"
              label="Task Name"
              type="search"
              className={classes.field}
              helper="normal"
            />
            <Field
              name="isActive"
              component={Switch}
              label="Active"
              className={classes.field}
            />
          </Grid>
          <Grid item xs={12} className={classes.row}>
            <Field
              name="categoryId"
              component={Select}
              items={categories}
              fullWidth
              label="Category"
              className={classes.field}
            />
            <Field
              name="subcategoryId"
              component={Select}
              items={subcategories.filter(subcat => {
                return subcat.categoryId === this.props.values.categoryId;
              })}
              fullWidth
              label="Subcategory"
              className={classes.field}
            />
            <Tooltip title="Edit Categories">
              <IconButton onClick={editCategories} aria-label="Edit Categories">
                <Edit />
              </IconButton>
            </Tooltip>
          </Grid>          
          <Grid item xs={12} className={classes.row}>
            <Typography
              color="error"
              variant="button"
              className={classes.field}
            >
              {errors.submit}
            </Typography>
            <div>
              <Button
                type="submit"
                color="primary"
                disabled={isSubmitting}
                variant="contained"
                className={classes.button}
              >
                Save
              </Button>
              <Button
                onClick={() => {
                  resetForm(initialValues);
                }}
                disabled={isSubmitting}
                color="secondary"
                variant="text"
                className={classes.button}
              >
                Reset
              </Button>
            </div>
          </Grid>        
        </Grid>
      </Form>
    );
  }
}

Task.propTypes = {
  classes: PropTypes.object.isRequired,
  label: PropTypes.string.isRequired,
  deleteTask: PropTypes.func,
  type: PropTypes.string.isRequired,
  categories: PropTypes.array.isRequired,
  subcategories: PropTypes.array.isRequired,
  isSubmitting: PropTypes.bool.isRequired,
  resetForm: PropTypes.func.isRequired,
  initialValues: PropTypes.object.isRequired,
  errors: PropTypes.object.isRequired,
  values: PropTypes.object,
  editCategories: PropTypes.func.isRequired
};

export default withStyles(styles)(Task);