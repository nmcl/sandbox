require 'singleton'
require 'java'

module Transaction
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

    jndiName = "java:comp/UserTransaction"

    def initialize (jndi = false)
      raise "JNDI not supported!" if (jndi)

      @transaction = com.arjuna.ats.jta.UserTransaction::userTransaction

      raise "Problem in acquiring UserTransaction handle!" if (@transaction == nil)
    end

    def begin
      @transaction.begin
    end

    def commit
      @transaction.commit
    end

    def rollback
      @transaction.rollback
    end

    def status
      return Status.new(@transaction.getStatus)
    end

    def makeRollbackOnly
      @transaction.setRollbackOnly
    end

    def timeout= (value)
      @transaction.setTransactionTimeout(value)
    end

    def to_s
      "UserTransactionSingleton"
    end
  end

  class UserTransaction
    def initialize (jndi)
      UserTransactionSingleton.instance.initialize(jndi)
    end

    def UserTransaction.userTransaction
      return UserTransactionSingleton.instance
    end
  end

  class TransactionManagerSingleton < UserTransactionSingleton
    include Singleton

    jndiName = "java:/TransactionManager"

    def initialize (jndi = false)
      raise "JNDI not supported!" if (jndi)
    
      @transaction = com.arjuna.ats.jta.TransactionManager::transactionManager

      raise "Problem in acquiring TransactionManager handle!" if (@transaction == nil)
    end

    def transaction
      txHandle = @transaction.getTransaction

      if (nil == txHandle)
        return nil
      else
        return Transaction.new(txHandle)
      end
    end
    
    def suspend
      txHandle = @transaction.suspend

      if (nil == txHandle)
        return nil
      else
        return Transaction.new(txHandle)
      end
    end

    def resume (obj)
      puts "resume =#{obj.theTransaction}"

      @transaction.resume(obj.theTransaction)
    end

    def to_s
      "TransactionManagerSingleton"
    end
  end

  class TransactionManager
    def initialize (jndi)
      TransactionManagerSingleton.instance.initialize(jndi)
    end

    def TransactionManager.transactionManager
      return TransactionManagerSingleton.instance
    end
  end

  class Transaction
    def initialize (tx)
      @_theTransaction = tx
    end

    def commit
      @_theTransaction.commit
    end

    def rollback
      @_theTransaction.rollback
    end

    def status
      return Status.new(@_theTransaction.getStatus)
    end

    def makeRollbackOnly
      @_theTransaction.setRollbackOnly
    end

    def == (other)
          puts "comparison =#{@_theTransaction} and #{other} and #{@_theTransaction.equals(other)}"
      return @_theTransaction.equals(other.theTransaction)
    end

    def to_s
      "Transaction: #{@_theTransaction}"
    end

    def theTransaction
      return @_theTransaction
    end
  end
end
