class Constants
{
	public static final int    numberOfBitmaps = 6;
	public static final int    replySize = 128;
	public static final int    ArjunaPort = 5010;
	public static final int    frameDuration = 500;
	public static final int    cashPointx = 80;
	public static final int    cashPointy = 170;
	public static final int	   pinSize = 4;
	public static final int    bitmapIndex = 0;
	public static final String ArjunaHost = "kellah";
}

class Opcodes
{
	public static final int OP_INSERT = 1;
	public static final int OP_WITHDRAW = 2;
	public static final int OP_INSPECT = 3;
	public static final int OP_DONE = 4;
	public static final int OP_NOTDONE = 5;
	public static final int OP_PREPARE = 6;
	public static final int OP_ABORT = 7;
	public static final int OP_COMMIT = 8;
	public static final int OP_TRANSFER = 9;
}

class BankOutcome
{
	public static final int DONE = 0;
	public static final int NOTDONE = 1;
	public static final int UNKNOWN = 2;
	public static final int INSUFFICIENT_FUNDS = 3;
	public static final int ACCOUNT_ERROR = 4;
}


