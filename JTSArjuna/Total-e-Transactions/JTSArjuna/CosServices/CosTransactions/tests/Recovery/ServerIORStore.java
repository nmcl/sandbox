//
// Copyright (C) 2000,
//
// Arjuna Solutions, Ltd., 
// Newcastle upon Tyne,
// Tyne and Wear,
// UK.
//

import java.util.Properties;
import java.io.*;

public class ServerIORStore
{
    public static void storeIOR(String serverName, String serverIOR)
        throws Exception
    {
        Properties serverIORs = new Properties();

        try
        {
            FileInputStream serverIORsFileInputStream = new FileInputStream("ServerIORs");
            serverIORs.load(serverIORsFileInputStream);
            serverIORsFileInputStream.close();
        }
        catch (Exception exception)
        {
        }

        serverIORs.put(serverName, serverIOR);

        FileOutputStream serverIORsFileOutputStream = new FileOutputStream("ServerIORs");
        serverIORs.store(serverIORsFileOutputStream, "Server IORs");
        serverIORsFileOutputStream.close();
    }

    public static void removeIOR(String serverName)
        throws Exception
    {
        Properties serverIORs = new Properties();

        FileInputStream serverIORsFileInputStream = new FileInputStream("ServerIORs");
        serverIORs.load(serverIORsFileInputStream);
        serverIORsFileInputStream.close();

        serverIORs.remove(serverName);

        FileOutputStream serverIORsFileOutputStream = new FileOutputStream("ServerIORs");
        serverIORs.store(serverIORsFileOutputStream, "Server IORs");
        serverIORsFileOutputStream.close();
    }

    public static String loadIOR(String serverName)
        throws Exception
    {
        String serverIOR = null;

        Properties serverIORs = new Properties();

        FileInputStream serverIORsFileInputStream = new FileInputStream("ServerIORs");
        serverIORs.load(serverIORsFileInputStream);
        serverIORsFileInputStream.close();

        serverIOR = (String) serverIORs.get(serverName);

        return serverIOR;
    }

    public static void remove()
    {
        try
	{
            File file = new File("ServerIORs");

            file.delete();
        }
        catch (Exception exception)
        {
            System.err.println("Failed to remove \"ServerIORs\": " + exception);
        }
    }
}
