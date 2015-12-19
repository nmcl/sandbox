require 'UserTransaction'
require 'TransactionManager'
require 'Utils'

theTransaction = UserTransaction.getUserTransaction
theTransaction.begin

value = theTransaction.getStatus

puts "begin #{value}"

theStatus = Status.new(value)

puts "begun transaction: #{theStatus}"

theTransaction.commit

theTransaction = TransactionManager.getTransactionManager
theTransaction.begin

value = theTransaction.getStatus

puts "begin #{value}"

theStatus = Status.new(value)

puts "begun transaction: #{theStatus}"

tx = theTransaction.suspend

puts "transaction #{tx}"

value = theTransaction.getStatus

theStatus = Status.new(value)

puts "and status #{theStatus}"

theTransaction.resume(tx)

theTransaction.commit
