require 'singleton'
require 'java'
require 'Exceptions'

class UserTransactionSingleton
  include Singleton

  def initialize (jndi = false)
    raise "JNDI not supported!" if (jndi)
    
    @transaction = com.arjuna.ats.jta.UserTransaction::userTransaction

    raise "Problem in acquiring UserTransaction handle!" if (@transaction == nil)
  end

  def begin
    @transaction.begin
  rescue => ex
    raise NotSupportedException.new("Nested transactions not supported!") if (ex.class == NativeException) && (ex.message.include? "javax.transaction.NotSupportedException")

    raise "Caught exception #{ex.class}: #{ex.message}"
  end

  def commit
    @transaction.commit
  rescue => ex
    raise RollbackException.new("Transaction rolled back!") if (ex.class == NativeException) && (ex.message.include? "javax.transaction.RollbackException")

    raise HeuristicMixedException.new("Transaction committed but with mixed outcome!") if (ex.class == NativeException) && (ex.message.include? "javax.transaction.HeuristicMixedException")

    raise HeuristicRollbackException.new("Transaction heuristically rolled back!") if (ex.class == NativeException) && (ex.message.include? "javax.transaction.HeuristicRollbackException")

    raise IllegalStateException.new("Transaction not in the right state to commit!") if (ex.class == NativeException) && (ex.message.include? "IllegalStateException")

    raise SecurityError.new("Not allowed to commit!") if (ex.class == NativeException) && (ex.message.include? "SecurityException")

    raise "Caught exception #{ex.class}: #{ex.message}"
  end

  def rollback
    @transaction.rollback
  rescue => ex
    raise IllegalStateException.new("Transaction not in the right state to commit!") if (ex.class == NativeException) && (ex.message.include? "IllegalStateException")

    raise SecurityError.new("Not allowed to commit!") if (ex.class == NativeException) && (ex.message.include? "SecurityException")

    raise "Caught exception #{ex.class}: #{ex.message}"    
  end

  def getStatus
    return @transaction.getStatus
  rescue => ex
    raise "Caught exception #{ex.class}: #{ex.message}"
  end

  def setRollbackOnly
    @transaction.setRollbackOnly
  rescue => ex
    raise IllegalStateException.new("Transaction not in the right state to commit!") if (ex.class == NativeException) && (ex.message.include? "IllegalStateException")

    raise "Caught exception #{ex.class}: #{ex.message}"
  end

  def setTransactionTimeout (timeout)
    @transaction.setTransactionTimeout(timeout)
  rescue => ex
    raise "Caught exception #{ex.class}: #{ex.message}"
  end

  def to_s
    "UserTransactionSingleton"
  end
end

class UserTransaction
  def initialize (jndi)
    UserTransactionSingleton.instance.initialize(jndi)
  end

  def UserTransaction.getUserTransaction
    return UserTransactionSingleton.instance
  end
end
