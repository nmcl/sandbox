require 'singleton'
require 'java'

module Transaction
  class NotSupportedException < StandardError
  end

  class RollbackException < StandardError
  end

  class HeuristicMixedException < StandardError
  end

  class HeuristicRollbackException < StandardError
  end

  class IllegalStateException < StandardError
  end

  class InvalidTransactionException < StandardError
  end

  class Status
    def initialize (st)
      @theStatus = st
    end

    def to_s
      return com.arjuna.ats.jta.utils.JTAHelper::stringForm(@theStatus.to_int)
    rescue
      raise "Invalid parameter #{@theStatus}"
    end

    :theStatus
  end

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

  class Transaction
    def initialize (tx)
      @_theTransaction = tx
    end

    def commit
      @_theTransaction.commit
    rescue => ex
      raise RollbackException.new("Transaction rolled back!") if (ex.class == NativeException) && (ex.message.include? "javax.transaction.RollbackException")

      raise HeuristicMixedException.new("Transaction committed but with mixed outcome!") if (ex.class == NativeException) && (ex.message.include? "javax.transaction.HeuristicMixedException")

      raise HeuristicRollbackException.new("Transaction heuristically rolled back!") if (ex.class == NativeException) && (ex.message.include? "javax.transaction.HeuristicRollbackException")

      raise IllegalStateException.new("Transaction not in the right state to commit!") if (ex.class == NativeException) && (ex.message.include? "IllegalStateException")
      
      raise SecurityError.new("Not allowed to commit!") if (ex.class == NativeException) && (ex.message.include? "SecurityException")
      
      raise "Caught exception #{ex.class}: #{ex.message}"
    end

    def rollback
      @_theTransaction.rollback
    rescue => ex
      raise IllegalStateException.new("Transaction not in the right state to commit!") if (ex.class == NativeException) && (ex.message.include? "IllegalStateException")

      raise SecurityError.new("Not allowed to commit!") if (ex.class == NativeException) && (ex.message.include? "SecurityException")

      raise "Caught exception #{ex.class}: #{ex.message}"    
    end

    def getStatus
      return @_theTransaction.getStatus
    rescue => ex
      raise "Caught exception #{ex.class}: #{ex.message}"
    end

    def setRollbackOnly
      @_theTransaction.setRollbackOnly
    rescue => ex
      raise IllegalStateException.new("Transaction not in the right state to commit!") if (ex.class == NativeException) && (ex.message.include? "IllegalStateException")

      raise "Caught exception #{ex.class}: #{ex.message}"
    end

    def to_s
      "Transaction: #{@_theTransaction}"
    end

    def getTheTransaction
      return @_theTransaction
    end
  end
end

