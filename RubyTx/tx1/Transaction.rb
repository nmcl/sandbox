require 'java'
require 'Exceptions'

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
