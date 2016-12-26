//
// Copyright (C) 2000,
//
// Arjuna Solutions, Ltd.,
// Newcastle upon Tyne,
// Tyne and Wear,
// UK.
//

import java.io.File;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCore.Common.*;
import com.arjuna.OrbCommon.*;
import java.sql.*;
import com.arjuna.JDBC2.Implementation.ObjectStore.JDBCAccess;

public class EmptyObjectStore
{
    public static void main(String[] args)
    {
        try
        {
            ORBInterface.initORB(args, null);
            OAInterface.initOA();

			if(PropertyManager.getProperty(ArjunaCoreEnvironment.OBJECTSTORE_TYPE) != null &&
			   PropertyManager.getProperty(ArjunaCoreEnvironment.OBJECTSTORE_TYPE).startsWith("JDBCStore"))
			{
				JDBCAccess mJDBC = (JDBCAccess) Class.forName(System.getProperty("JDBC2_USER_DB_ACCESS")).newInstance();
				Connection mConnection = mJDBC.getConnection();
				Statement s = mConnection.createStatement();
				s.executeUpdate("DROP TABLE " + mJDBC.tableName());
			}
			else
			{
				String objectStoreDirName = PropertyManager.getProperty(ArjunaCoreEnvironment.OBJECTSTORE_DIR, com.arjuna.ArjunaCore.Common.Configuration.objectStoreRoot());

				if (objectStoreDirName != null)
				{
				    File objectStoreDir = new File(objectStoreDirName);

				    removeContents(objectStoreDir);
				}
				else
				{
				    System.err.println("Unable to find the ObjectStore root.");
				}
			}
        }
        catch (Exception exception)
        {
            System.err.println("EmptyObjectStore.main: " + exception);
            exception.printStackTrace(System.err);
        }

        try
        {
            OAInterface.shutdownOA();
            ORBInterface.shutdownORB();
        }
        catch (Exception exception)
        {
            System.err.println("EmptyObjectStore.main: " + exception);
            exception.printStackTrace(System.err);
        }
    }

    public static void removeContents(File directory)
    {
        if ((directory != null) &&
            directory.isDirectory() &&
            (! directory.getName().equals("")) &&
            (! directory.getName().equals("/")) &&
            (! directory.getName().equals("\\")) &&
            (! directory.getName().equals(".")) &&
            (! directory.getName().equals("..")))
        {
            File[] contents = directory.listFiles();

            for (int index = 0; index < contents.length; index++)
            {
                if (contents[index].isDirectory())
                {
                    removeContents(contents[index]);

                    //System.err.println("Deleted: " + contents[index]);
                    contents[index].delete();
                }
                else
				{
                    System.err.println("Deleted: " + contents[index]);
                    contents[index].delete();
                }
            }
        }
    }
}
