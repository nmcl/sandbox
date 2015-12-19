require File.dirname(__FILE__) +  '/spec_helper.rb'

describe "Core transaction tests" do
  it "should execute without any problems" do

    theTransaction = Transaction::UserTransaction.userTransaction
    theTransaction.begin

    value = theTransaction.status

    puts "begin #{value}"

    theTransaction.commit

    theTransaction = Transaction::TransactionManager.transactionManager
    theTransaction.begin

    value = theTransaction.status

    puts "begin #{value}"

    tx = theTransaction.suspend

    puts "transaction #{tx}"

    value = theTransaction.status

    puts "and status #{value}"

    theTransaction.resume(tx)

    theTransaction.commit
  end

  it "should throw NotSupportedException when a second transaction is begun" do

    theTransaction = Transaction::UserTransaction.userTransaction
    theTransaction.begin

    lambda{theTransaction.begin}.should raise_error

    theTransaction.rollback
  end

  it "should suspend and resume transaction" do
    theTransaction = Transaction::TransactionManager.transactionManager
    theTransaction.begin

    txHandle = theTransaction.suspend

    puts "suspended transaction = #{txHandle}"

    raise "Transaction invalid" if nil == txHandle

    currentHandle = theTransaction.transaction

    raise "Transaction still associated with thread" if nil != currentHandle

    theTransaction.resume(txHandle)

    puts "transaction after resume = #{theTransaction.transaction}"

    raise "Transactions not equal" if theTransaction.transaction != txHandle

    theTransaction.rollback
  end

  it "should commit" do
    theTransaction = Transaction::TransactionManager.transactionManager
    theTransaction.begin

    theTransaction.commit

    raise "Transaction still associated with thread" if nil != theTransaction.transaction
  end

  it "should rollback" do
    theTransaction = Transaction::TransactionManager.transactionManager
    theTransaction.begin

    theTransaction.rollback

    raise "Transaction still associated with thread" if nil != theTransaction.transaction
  end

  it "should commit and remain associated with the thread" do
    theTransaction = Transaction::TransactionManager.transactionManager
    theTransaction.begin

    txHandle = theTransaction.transaction
    txHandle.commit

    raise "Transaction not associated with thread" if nil == theTransaction.transaction

    theTransaction.suspend
  end

  it "should throw IllegalStateException" do
    theTransaction = Transaction::TransactionManager.transactionManager
    theTransaction.begin

    txHandle = theTransaction.transaction
    txHandle.commit

    raise "Transaction not associated with thread" if nil == theTransaction.transaction

    lambda{theTransaction.rollback}.should raise_error
  end
end
