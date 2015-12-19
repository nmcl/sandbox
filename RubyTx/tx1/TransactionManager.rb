require 'java'
require 'UserTransaction'
require 'Transaction'
require 'singleton'

class TransactionManagerSingleton < UserTransactionSingleton
  include Singleton

  def initialize (jndi = false)
    raise "JNDI not supported!" if (jndi)
    
    @transaction = com.arjuna.ats.jta.TransactionManager::transactionManager

    raise "Problem in acquiring TransactionManager handle!" if (@transaction == nil)
  end

  def getTransaction
    return Transaction.new(@transaction.getTransaction)
  rescue => ex
    raise "Caught exception #{ex.class}: #{ex.message}"
  end

  def suspend
    return Transaction.new(@transaction.suspend)
  rescue => ex
    raise "Caught exception #{ex.class}: #{ex.message}"
  end

  def resume (obj)
    @transaction.resume(obj.getTheTransaction)
  rescue => ex
    raise InvalidTransactionException.new("Transaction invalid!") if (ex.class == NativeException) && (ex.message.include? "javax.transaction.InvalidTransactionException")

    raise IllegalStateException.new("Transaction not in the right state to commit!") if (ex.class == NativeException) && (ex.message.include? "IllegalStateException")

    raise "Caught exception #{ex.class}: #{ex.message}" if (ex.class != RuntimeError)
  end

  def to_s
    "TransactionManagerSingleton"
  end
end

class TransactionManager
  def initialize (jndi)
    TransactionManagerSingleton.instance.initialize(jndi)
  end

  def TransactionManager.getTransactionManager
    return TransactionManagerSingleton.instance
  end
end


