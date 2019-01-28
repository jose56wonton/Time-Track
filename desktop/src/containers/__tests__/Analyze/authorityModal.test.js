import React from 'react';
import { shallow } from 'enzyme';

import AuthorityModalHOC, { AuthorityModal } from 'containers/Analyze/authorityModalContainer';

const props =  {  
  classes: {}
};

const setup = overRides => {  
  return shallow(<AuthorityModal {...props} {...overRides}/>);    
};

const setupHOC = overRides => {  
  return shallow(<AuthorityModalHOC {...props} {...overRides}/>);    
};


describe('Authority Modal Container', () => {  
  afterEach(() => {
    jest.clearAllMocks();
  });
  it('should render correctly', () => {
    setup();       
  });  
  it('should render correctly withStyles', () => {
    setupHOC();
  });
});