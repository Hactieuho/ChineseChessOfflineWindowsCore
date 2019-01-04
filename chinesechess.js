var ffi = require('../../')

var libchinesechess = ffi.Library('./libChineseChess', {
  'main': [ 'int', []]
})

var output = libchinesechess.main()

console.log('Your output: ' + output)
