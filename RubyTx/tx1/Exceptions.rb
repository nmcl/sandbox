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
