import React, { Component } from 'react';
import PropTypes from 'prop-types';
import { connect } from 'react-redux';
import { bindActionCreators } from 'redux';

import moment from 'moment';

import { employeeActions, taskActions, projectActions, shiftActions, analyzeActions } from 'store/actions';
import { shiftSelectors } from 'store/selectors';
import NewTable from 'components/tables/NewTable';
import Progress from 'components/helpers/Progress';
import * as TableDataTypes from 'constants/tableDataTypes';
import { analyzeStatus } from 'constants/analyze';
import domain from 'constants/domains';

export class ShiftIndex extends Component {
  componentDidMount = () => {
    // Fetching here to ensure that all employees have been fetched before we try and display their name for their shift
    this.props.getAllEmployees();
    this.props.getAllProjects();
    this.props.getAllTasks();
    this.props.getShiftsInRange(moment().subtract(400, 'days').format('MM-DD-YY HH:mm:ss'), moment().add(14,'days').format('MM-DD-YY HH:mm:ss'));
  }

  selectLabel = selected =>`${selected.employee.firstName} ${selected.employee.lastName}'s shift selected`;

  select = object => this.props.select(domain.SHIFT,object)

  add = () => this.props.setStatus(domain.SHIFT,analyzeStatus.ADDING)

  render() {
    const { shifts, selected } = this.props;
    
    if (!shifts) return <Progress variant="circular" fullWidth fullHeight />;
    
    return (     
      <NewTable
        // selectLabel={this.selectLabel}
        // label="Shifts"
        data={shifts || []}
        columns={rows} 
        selected={selected}
        select={this.select}
        // add={this.add}
      />      
    );
  }
}

ShiftIndex.propTypes = {
  shifts: PropTypes.array,
  getShiftsInRange: PropTypes.func.isRequired,
  getAllEmployees: PropTypes.func.isRequired,
  getAllProjects: PropTypes.func.isRequired,
  getAllTasks: PropTypes.func.isRequired,
  select: PropTypes.func.isRequired,
  setStatus: PropTypes.func.isRequired,
  selected: PropTypes.object
};

/* istanbul ignore next */
const mapStateToProps = state => {
  return {
    shifts: shiftSelectors.getShiftsInRange(state, { startTime: moment().subtract(400, 'days').format('MM-DD-YY HH:mm:ss'), endTime: moment().add(14,'days').format('MM-DD-YY HH:mm:ss') }),
    selected: shiftSelectors.getSelectedShift(state)
  };
};

/* istanbul ignore next */
const mapDispatchToProps = dispatch => {
  return {
    getAllEmployees: () => {
      return dispatch(employeeActions.getAllEmployees());
    },
    getAllProjects: () => {
      return dispatch(projectActions.getAllProjects());
    },
    getAllTasks: () => {
      return dispatch(taskActions.getAllTasks());
    },
    getShiftsInRange: (start, end) => {
      return dispatch(shiftActions.getShiftsInRange(start, end));
    },    
    ...bindActionCreators({ ...analyzeActions }, dispatch)   
  };
};

export default connect(mapStateToProps, mapDispatchToProps)(ShiftIndex);

const rows = [
  {
    id: 'employee',   
    //width: 150, 
    height: 56,
    padding: 'dense',
    label: 'First Name',
    type: TableDataTypes.OBJECT,
    keys: ['firstName']
  },
  {
    id: 'employee',   
    //width: 150,   
    height: 56,
    padding: 'dense',
    label: 'Last Name',
    type: TableDataTypes.OBJECT,
    keys: ['lastName']
  },
  {
    id: 'clockInDate',   
    //width: 150,   
    height: 56,
    padding: 'dense',
    label: 'Clock In',
    type: TableDataTypes.DATETIME
  },
  {
    id: 'clockOutDate',  
    //width: 150,  
    height: 56,  
    padding: 'dense',
    label: 'Clock Out',
    type: TableDataTypes.DATETIME
  },
  {
    id: 'length',     
    //width: 150, 
    height: 56,
    padding: 'dense',
    label: 'Length',
    type: TableDataTypes.LENGTH
  }
];