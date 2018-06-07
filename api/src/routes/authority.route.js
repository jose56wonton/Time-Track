module.exports = function (router) {
  const authority = require('../controllers/authority.controller');
  const models = require('../models')

  router.post('/authority', authority.create);
  router.get('/authority',authority.find);
  router.put('/authority', authority.update);
  router.delete('/authority', authority.destroy);
};