const transparentWindow = require('./build/Release/transparent_window.node')

module.exports = {
  create(callback = function(){}) {
    transparentWindow.create(callback)
  },
  close() {
    transparentWindow.close()
  }
}