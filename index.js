const transparentWindow = require('./build/Release/transparent_window.node')

module.exports = {
  // callback receive mouse relative movement
  create(callback = function({x, y}){}) {
    transparentWindow.create(callback)
  },
  close() {
    transparentWindow.close()
  },
  // topmost() {
  //   transparentWindow.topmost()
  // }
}