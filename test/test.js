const assert = require('assert')
const transparentWindow = require('../index')

describe('transparent window', () => {
  it('given a normal case, when create and close repeatly, should work well', (done) => {
    transparentWindow.create(() => {
      transparentWindow.close()
      const timer = setTimeout(() => {
        clearTimeout(timer)
        transparentWindow.create(() => {
          transparentWindow.close()
          done()
        })
      }, 10)
    })
  })
})