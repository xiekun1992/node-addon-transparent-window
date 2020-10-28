const transparentWindow = require('./build/Release/transparent_window.node')

module.exports = {
  create() {
    transparentWindow.create()
  },
  close() {
    transparentWindow.close()
  }
}