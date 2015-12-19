require 'java'

$: << File.dirname(__FILE__) + '/../' 

require 'transaction'

TEST_DATA_BASE = 'target/test-data'
TEST_DATA_DIR = File.dirname(__FILE__) + "/../#{TEST_DATA_BASE}"
