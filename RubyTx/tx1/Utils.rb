require 'java'

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
