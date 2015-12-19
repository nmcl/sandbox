require 'Transaction'

theTransaction = Transaction::UserTransaction.getUserTransaction
theTransaction.begin

value = theTransaction.getStatus

puts "begin #{value}"

theStatus = Transaction::Status.new(value)

puts "begun transaction: #{theStatus}"

theTransaction.commit

theTransaction = Transaction::TransactionManager.getTransactionManager
theTransaction.begin

value = theTransaction.getStatus

puts "begin #{value}"

theStatus = Transaction::Status.new(value)

puts "begun transaction: #{theStatus}"

tx = theTransaction.suspend

puts "transaction #{tx}"

value = theTransaction.getStatus

theStatus = Transaction::Status.new(value)

puts "and status #{theStatus}"

theTransaction.resume(tx)

theTransaction.commit
