/*
 * Template JAVA User Interface
 * =============================
 *
 * Database Management Systems
 * Department of Computer Science &amp; Engineering
 * University of California - Riverside
 *
 * Target DBMS: 'Postgres'
 *
 */


import java.sql.DriverManager;
import java.sql.Connection;
import java.sql.Statement;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.io.File;
import java.io.FileReader;
import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.util.List;
import java.util.ArrayList;
import java.lang.Math;

/**
 * This class defines a simple embedded SQL utility class that is designed to
 * work with PostgreSQL JDBC drivers.
 *
 */
public class PizzaStore {

   // reference to physical database connection.
   private Connection _connection = null;

   // handling the keyboard inputs through a BufferedReader
   // This variable can be global for convenience.
   static BufferedReader in = new BufferedReader(
                                new InputStreamReader(System.in));

   /**
    * Creates a new instance of PizzaStore
    *
    * @param hostname the MySQL or PostgreSQL server hostname
    * @param database the name of the database
    * @param username the user name used to login to the database
    * @param password the user login password
    * @throws java.sql.SQLException when failed to make a connection.
    */
   public PizzaStore(String dbname, String dbport, String user, String passwd) throws SQLException {

      System.out.print("Connecting to database...");
      try{
         // constructs the connection URL
         String url = "jdbc:postgresql://localhost:" + dbport + "/" + dbname;
         System.out.println ("Connection URL: " + url + "\n");

         // obtain a physical connection
         this._connection = DriverManager.getConnection(url, user, passwd);
         System.out.println("Done");
      }catch (Exception e){
         System.err.println("Error - Unable to Connect to Database: " + e.getMessage() );
         System.out.println("Make sure you started postgres on this machine");
         System.exit(-1);
      }//end catch
   }//end PizzaStore

   /**
    * Method to execute an update SQL statement.  Update SQL instructions
    * includes CREATE, INSERT, UPDATE, DELETE, and DROP.
    *
    * @param sql the input SQL string
    * @throws java.sql.SQLException when update failed
    */
   public void executeUpdate (String sql) throws SQLException {
      // creates a statement object
      Statement stmt = this._connection.createStatement ();

      // issues the update instruction
      stmt.executeUpdate (sql);

      // close the instruction
      stmt.close ();
   }//end executeUpdate

   /**
    * Method to execute an input query SQL instruction (i.e. SELECT).  This
    * method issues the query to the DBMS and outputs the results to
    * standard out.
    *
    * @param query the input query string
    * @return the number of rows returned
    * @throws java.sql.SQLException when failed to execute the query
    */
   public int executeQueryAndPrintResult (String query) throws SQLException {
      // creates a statement object
      Statement stmt = this._connection.createStatement ();

      // issues the query instruction
      ResultSet rs = stmt.executeQuery (query);

      /*
       ** obtains the metadata object for the returned result set.  The metadata
       ** contains row and column info.
       */
      ResultSetMetaData rsmd = rs.getMetaData ();
      int numCol = rsmd.getColumnCount ();
      int rowCount = 0;

      // iterates through the result set and output them to standard out.
      boolean outputHeader = true;
      while (rs.next()){
		 if(outputHeader){
			for(int i = 1; i <= numCol; i++){
			System.out.print(rsmd.getColumnName(i) + "\t");
			}
			System.out.println();
			outputHeader = false;
		 }
         for (int i=1; i<=numCol; ++i)
            System.out.print (rs.getString (i) + "\t");
         System.out.println ();
         ++rowCount;
      }//end while
      stmt.close();
      return rowCount;
   }//end executeQuery

   /**
    * Method to execute an input query SQL instruction (i.e. SELECT).  This
    * method issues the query to the DBMS and returns the results as
    * a list of records. Each record in turn is a list of attribute values
    *
    * @param query the input query string
    * @return the query result as a list of records
    * @throws java.sql.SQLException when failed to execute the query
    */
   public List<List<String>> executeQueryAndReturnResult (String query) throws SQLException {
      // creates a statement object
      Statement stmt = this._connection.createStatement ();

      // issues the query instruction
      ResultSet rs = stmt.executeQuery (query);

      /*
       ** obtains the metadata object for the returned result set.  The metadata
       ** contains row and column info.
       */
      ResultSetMetaData rsmd = rs.getMetaData ();
      int numCol = rsmd.getColumnCount ();
      int rowCount = 0;

      // iterates through the result set and saves the data returned by the query.
      boolean outputHeader = false;
      List<List<String>> result  = new ArrayList<List<String>>();
      while (rs.next()){
        List<String> record = new ArrayList<String>();
		for (int i=1; i<=numCol; ++i)
			record.add(rs.getString (i));
        result.add(record);
      }//end while
      stmt.close ();
      return result;
   }//end executeQueryAndReturnResult

   /**
    * Method to execute an input query SQL instruction (i.e. SELECT).  This
    * method issues the query to the DBMS and returns the number of results
    *
    * @param query the input query string
    * @return the number of rows returned
    * @throws java.sql.SQLException when failed to execute the query
    */
   public int executeQuery (String query) throws SQLException {
       // creates a statement object
       Statement stmt = this._connection.createStatement ();

       // issues the query instruction
       ResultSet rs = stmt.executeQuery (query);

       int rowCount = 0;

       // iterates through the result set and count nuber of results.
       while (rs.next()){
          rowCount++;
       }//end while
       stmt.close ();
       return rowCount;
   }

   /**
    * Method to fetch the last value from sequence. This
    * method issues the query to the DBMS and returns the current
    * value of sequence used for autogenerated keys
    *
    * @param sequence name of the DB sequence
    * @return current value of a sequence
    * @throws java.sql.SQLException when failed to execute the query
    */
   public int getCurrSeqVal(String sequence) throws SQLException {
	Statement stmt = this._connection.createStatement ();

	ResultSet rs = stmt.executeQuery (String.format("Select currval('%s')", sequence));
	if (rs.next())
		return rs.getInt(1);
	return -1;
   }

   /**
    * Method to close the physical connection if it is open.
    */
   public void cleanup(){
      try{
         if (this._connection != null){
            this._connection.close ();
         }//end if
      }catch (SQLException e){
         // ignored.
      }//end try
   }//end cleanup

   /**
    * The main execution method
    *
    * @param args the command line arguments this inclues the <mysql|pgsql> <login file>
    */
   public static void main (String[] args) {
      if (args.length != 3) {
         System.err.println (
            "Usage: " +
            "java [-classpath <classpath>] " +
            PizzaStore.class.getName () +
            " <dbname> <port> <user>");
         return;
      }//end if

      Greeting();
      PizzaStore esql = null;
      try{
         // use postgres JDBC driver.
         Class.forName ("org.postgresql.Driver").newInstance ();
         // instantiate the PizzaStore object and creates a physical
         // connection.
         String dbname = args[0];
         String dbport = args[1];
         String user = args[2];
         esql = new PizzaStore (dbname, dbport, user, "");

         boolean keepon = true;
         while(keepon) {
            // These are sample SQL statements
            System.out.println("MAIN MENU");
            System.out.println("---------");
            System.out.println("1. Create user");
            System.out.println("2. Log in");
            System.out.println("9. < EXIT");
            String authorisedUser = null;
            switch (readChoice()){
               case 1: CreateUser(esql); break;
               case 2: authorisedUser = LogIn(esql); break;
               case 9: keepon = false; break;
               default : System.out.println("Unrecognized choice!"); break;
            }//end switch
            if (authorisedUser != null) {
              boolean usermenu = true;
              while(usermenu) {
                System.out.println("MAIN MENU");
                System.out.println("---------");
                System.out.println("1. View Profile");
                System.out.println("2. Update Profile");
                System.out.println("3. View Menu");
                System.out.println("4. Place Order"); //make sure user specifies which store
                System.out.println("5. View Full Order ID History");
                System.out.println("6. View Past 5 Order IDs");
                System.out.println("7. View Order Information"); //user should specify orderID and then be able to see detailed information about the order
                System.out.println("8. View Stores"); 

                //**the following functionalities should only be able to be used by drivers & managers**
                System.out.println("9. Update Order Status");

                //**the following functionalities should ony be able to be used by managers**
                System.out.println("10. Update Menu");
                System.out.println("11. Update User");

                System.out.println(".........................");
                System.out.println("20. Log out");
                switch (readChoice()){
                   case 1: viewProfile(esql, authorisedUser); break;
                   case 2: updateProfile(esql, authorisedUser); break;
                   case 3: viewMenu(esql); break;
                   case 4: placeOrder(esql, authorisedUser); break;
                   case 5: viewAllOrders(esql, authorisedUser); break;
                   case 6: viewRecentOrders(esql, authorisedUser); break;
                   case 7: viewOrderInfo(esql, authorisedUser); break;
                   case 8: viewStores(esql); break;
                   case 9: updateOrderStatus(esql, authorisedUser); break;
                   case 10: updateMenu(esql, authorisedUser); break;
                   case 11: updateUser(esql, authorisedUser); break;

                   case 20: usermenu = false; break;
                   default : System.out.println("Unrecognized choice!"); break;
                }
              }
            }
         }//end while
      }catch(Exception e) {
         System.err.println (e.getMessage ());
      }finally{
         // make sure to cleanup the created table and close the connection.
         try{
            if(esql != null) {
               System.out.print("Disconnecting from database...");
               esql.cleanup ();
               System.out.println("Done\n\nBye !");
            }//end if
         }catch (Exception e) {
            // ignored.
         }//end try
      }//end try
   }//end main

   public static void Greeting(){
      System.out.println(
         "\n\n-----------------------------------------------------------\n" +
         "Welcome to Pizza Store! What would you like to order today?\n" +
         "-----------------------------------------------------------\n");
   }//end Greeting

   /*
    * Reads the users choice given from the keyboard
    * @int
    **/
   public static int readChoice() {
      int input;
      // returns only if a correct value is given.
      do {
         System.out.print("Please make your choice: ");
         try { // read the integer, parse it and break.
            input = Integer.parseInt(in.readLine());
            break;
         }catch (Exception e) {
            System.out.println("Your input is invalid!");
            continue;
         }//end try
      }while (true);
      return input;
   }//end readChoice

   /*
    * Creates a new user
    **/
   public static void CreateUser(PizzaStore esql) {
      try {
         System.out.print("Enter username: ");
         String login = in.readLine().trim();

         System.out.print("Enter password: ");
         String password = in.readLine().trim(); 

         System.out.print("Enter phone number: ");
         String phoneNum = in.readLine().trim();

         if (login.isEmpty() || password.isEmpty() || phoneNum.isEmpty()) {
            System.out.println("Error: All fields must be filled.");
            return;
         }

         String checkQuery = String.format(
            "SELECT COUNT(*) FROM Users WHERE login = '%s';",
            login
         );

         List<List<String>> userCheckResult = esql.executeQueryAndReturnResult(checkQuery);
         int userCount = Integer.parseInt(userCheckResult.get(0).get(0)); 

         if (userCount > 0) {
            System.out.println("Error: Username already exists. Please choose a different username.\n");
            return;
         }

         String insertQuery = String.format(
            "INSERT INTO Users (login, password, role, favoriteItems, phoneNum) " +
            "VALUES ('%s', '%s', 'Customer', NULL, '%s');",
            login, password, phoneNum
         );

         esql.executeUpdate(insertQuery);
         System.out.println("User successfully created!\n");

      } catch (Exception e) {
         System.err.println("Error: " + e.getMessage());
      }  
   }

  /*
    * Check log in credentials for an existing user
    * @return User login or null is the user does not exist
    **/
   public static String LogIn(PizzaStore esql) {
      try {
         // Prompt user for login credentials
         System.out.print("Enter username: ");
         String login = in.readLine().trim();

         System.out.print("Enter password: ");
         String password = in.readLine().trim();

         // Validate input
         if (login.isEmpty() || password.isEmpty()) {
            System.out.println("Error: Username and password cannot be empty.");
            return null;
         }

         // SQL query to check credentials
         String query = String.format(
            "SELECT login, role FROM Users WHERE login = '%s' AND password = '%s';",
            login, password
         );

         List<List<String>> result = esql.executeQueryAndReturnResult(query);

         // If user exists, login successful
         if (!result.isEmpty()) {
            System.out.println("Login successful! Welcome, " + login + " (" + result.get(0).get(1) + ")" + "\n");
            return login;  // Returning login for session tracking
         } else {
            System.out.println("Error: \n");
            return null;
         }

      } catch (Exception e) {
         System.err.println("Error: " + e.getMessage());
         return null;
      }
   }
  

// Rest of the functions definition go in here

   public static void viewProfile(PizzaStore esql, String authorisedUser) {
      System.out.println("\n--- Viewing Profile ---");

      try {
         // SQL query to retrieve user profile
         String query = String.format(
            "SELECT login, favoriteItems, phoneNum FROM Users WHERE login = '%s';",
            authorisedUser
         );

         // DEBUG: Print the query to check if it's correct
         System.out.println("Executing query: " + query);

         // Execute the query and store the result
         List<List<String>> result = esql.executeQueryAndReturnResult(query);

         // If user exists, display profile information
         if (!result.isEmpty()) {
            List<String> profile = result.get(0);
            System.out.println("\n--- Your Profile ---");
            System.out.println("Username: " + profile.get(0));
            System.out.println("Favorite Items: " + (profile.get(1) == null ? "None" : profile.get(1)));
            System.out.println("Phone Number: " + profile.get(2) + "\n");
         } else {
            System.out.println("Error: Profile not found.");
         }

      } catch (Exception e) {
         System.err.println("Error: " + e.getMessage());
      }
   }



   public static void updateProfile(PizzaStore esql, String authorisedUser) {
      try {
         System.out.println("\n--- Update Profile ---");

         // Get user's role to determine if they are a manager
         String roleQuery = String.format("SELECT role FROM Users WHERE login = '%s';", authorisedUser);
         List<List<String>> roleResult = esql.executeQueryAndReturnResult(roleQuery);
         if (roleResult.isEmpty()) {
            System.out.println("Error: Unable to retrieve your role.");
            return;
         }
         String userRole = roleResult.get(0).get(0);

         // Menu options
         System.out.println("What would you like to update?");
         System.out.println("1. Favorite Items");
         System.out.println("2. Phone Number");
         System.out.println("3. Password");
         if (userRole.equals("Manager")) {
            System.out.println("4. Update Another User’s Login/Role");
         }
         System.out.print("Enter your choice: ");
         int choice = Integer.parseInt(in.readLine().trim());

         String updateQuery = "";
         switch (choice) {
            case 1:
               System.out.print("Enter new favorite items: ");
               String newFav = in.readLine().trim();
               updateQuery = String.format("UPDATE Users SET favoriteItems = '%s' WHERE login = '%s';", newFav, authorisedUser);
               break;
            case 2:
               System.out.print("Enter new phone number: ");
               String newPhone = in.readLine().trim();
               updateQuery = String.format("UPDATE Users SET phoneNum = '%s' WHERE login = '%s';", newPhone, authorisedUser);
               break;
            case 3:
               System.out.print("Enter new password: ");
               String newPass = in.readLine().trim();
               updateQuery = String.format("UPDATE Users SET password = '%s' WHERE login = '%s';", newPass, authorisedUser);
               break;
            case 4:
               if (!userRole.equals("Manager")) {
                     System.out.println("Error: Only managers can update another user's login/role.");
                     return;
               }
               System.out.print("Enter the username to update: ");
               String targetUser = in.readLine().trim();
               System.out.print("Enter new login (or press Enter to keep unchanged): ");
               String newLogin = in.readLine().trim();
               System.out.print("Enter new role (Customer/Manager) (or press Enter to keep unchanged): ");
               String newRole = in.readLine().trim();

               // Build query dynamically
               if (!newLogin.isEmpty() && !newRole.isEmpty()) {
                     updateQuery = String.format("UPDATE Users SET login = '%s', role = '%s' WHERE login = '%s';", newLogin, newRole, targetUser);
               } else if (!newLogin.isEmpty()) {
                     updateQuery = String.format("UPDATE Users SET login = '%s' WHERE login = '%s';", newLogin, targetUser);
               } else if (!newRole.isEmpty()) {
                     updateQuery = String.format("UPDATE Users SET role = '%s' WHERE login = '%s';", newRole, targetUser);
               } else {
                     System.out.println("No changes made.");
                     return;
               }
               break;
            default:
               System.out.println("Invalid choice.");
               return;
         }

         // Execute the update query
         esql.executeUpdate(updateQuery);
         System.out.println("Profile successfully updated!");

      } catch (Exception e) {
            System.err.println("Error: " + e.getMessage());
      }
   }
  

   public static void viewMenu(PizzaStore esql) {
      try {
         System.out.println("\n--- Browse Menu ---");
         System.out.println("1. View all items");
         System.out.println("2. Filter by type ('entree', 'drinks', 'sides')");
         System.out.println("3. Filter by price (items under a certain price)");
         System.out.println("4. Sort by price (Low to High)");
         System.out.println("5. Sort by price (High to Low)");
         System.out.println("Enter your choice: ");
         
         int choice = Integer.parseInt(in.readLine().trim());
         String query = "SELECT * FROM Items";

         switch (choice) {
            case 1:
               // No changes, just display all menu items
               break;
            case 2:
               System.out.print("Enter item type ('entree', 'drinks', 'sides'): ");
               String type = in.readLine().trim();
               query += String.format(" WHERE TRIM(typeofitem) = '%s'", type);
               break;
            case 3:
               System.out.print("Enter maximum price: ");
               double maxPrice = Double.parseDouble(in.readLine().trim());
               query += String.format(" WHERE price <= %.2f", maxPrice);
               break;
            case 4:
               query += " ORDER BY price ASC";
               break;
            case 5:
               query += " ORDER BY price DESC";
               break;
            default:
               System.out.println("Invalid choice.");
               return;
         }

         // Execute query
         // esql.executeQueryAndPrintResult(query);
         // Execute query and get result
         List<List<String>> menuResults = esql.executeQueryAndReturnResult(query);
         System.out.printf("%-40s %-80s %-15s %-10s %-50s%n",
            "Item Name", "Ingredients", "Type", "Price", "Description");
         System.out.println("----------------------------------------------------------------------------------------------------------------------------------------------------------------");

         for (List<String> row : menuResults) {
            System.out.printf("%-40s %-80s %-15s %-10s %-50s%n",
                     row.get(0), row.get(1), row.get(2), row.get(3), row.get(4));
         }

      } catch (Exception e) {
            System.err.println("Error: " + e.getMessage());
      }
   }
  


   public static void placeOrder(PizzaStore esql, String authorisedUser) {
      try {
         System.out.println("\n--- Place Order ---");

         // Step 1: Ask user for store ID
         System.out.print("Enter the Store ID (1-1000) where you want to place your order: ");
         int storeID = Integer.parseInt(in.readLine().trim());

         // Step 2: Generate a unique orderID starting at 11000
         String getMaxOrderIDQuery = "SELECT COALESCE(MAX(orderID), 10999) + 1 FROM FoodOrder;";
         List<List<String>> orderIDResult = esql.executeQueryAndReturnResult(getMaxOrderIDQuery);
         int newOrderID = Integer.parseInt(orderIDResult.get(0).get(0));

         // Step 3: Ask user for item names and quantities
         List<String> items = new ArrayList<>();
         List<Integer> quantities = new ArrayList<>();
         String itemName;
         int quantity;

         while (true) {
            System.out.print("Enter item name (or type 'done' to finish): ");
            itemName = in.readLine().trim();
            if (itemName.equalsIgnoreCase("done")) break;

            // Validate if the item exists
            String checkItemQuery = String.format("SELECT COUNT(*) FROM Items WHERE itemName ILIKE '%s';", itemName);
            List<List<String>> itemCheck = esql.executeQueryAndReturnResult(checkItemQuery);
            if (Integer.parseInt(itemCheck.get(0).get(0)) == 0) {
               System.out.println("Error: Item not found in menu. Please enter a valid item.");
               continue;
            }

            System.out.print("Enter quantity: ");
            quantity = Integer.parseInt(in.readLine().trim());

            items.add(itemName);
            quantities.add(quantity);
         }

         // Step 4: Calculate total price
         StringBuilder priceQuery = new StringBuilder("SELECT SUM(price * quantity) FROM (VALUES ");
         for (int i = 0; i < items.size(); i++) {
            if (i > 0) priceQuery.append(", ");
            priceQuery.append(String.format("('%s', %d)", items.get(i), quantities.get(i)));
         }
         priceQuery.append(") AS oi(itemName, quantity) JOIN Items i ON i.itemName ILIKE oi.itemName;");

         List<List<String>> priceResult = esql.executeQueryAndReturnResult(priceQuery.toString());
         if (priceResult.isEmpty() || priceResult.get(0).get(0) == null) {
            System.out.println("Error: Could not compute total price. Check if the items exist.");
            return;
         }
         double totalPrice = Double.parseDouble(priceResult.get(0).get(0));

         // Step 5: Insert new order into FoodOrder
         String orderQuery = String.format(
            "INSERT INTO FoodOrder (orderID, login, storeID, totalPrice, orderTimestamp, orderStatus) " +
            "VALUES (%d, '%s', %d, %.2f, NOW(), 'Pending');",
            newOrderID, authorisedUser, storeID, totalPrice
         );
         esql.executeUpdate(orderQuery);

         // Step 6: Insert items into ItemsInOrder
         StringBuilder itemInsertQuery = new StringBuilder("INSERT INTO ItemsInOrder (orderID, itemName, quantity) VALUES ");
         for (int i = 0; i < items.size(); i++) {
            if (i > 0) itemInsertQuery.append(", ");
            itemInsertQuery.append(String.format("(%d, '%s', %d)", newOrderID, items.get(i), quantities.get(i)));
         }
         itemInsertQuery.append(";");
         esql.executeUpdate(itemInsertQuery.toString());

         // Step 7: Display total price
         System.out.printf("Order placed successfully! Your total price is: $%.2f%n", totalPrice, "\n");

      } catch (Exception e) {
            System.err.println("Error: " + e.getMessage());
      }
  }
  
  // other functions
   public static void viewAllOrders(PizzaStore esql, String authorisedUser) {
          try {
        System.out.println("\n--- Order History ---");

      String roleQuery = String.format("SELECT TRIM(BOTH FROM role) FROM Users WHERE login = '%s';", authorisedUser);

        List<List<String>> roleResult = esql.executeQueryAndReturnResult(roleQuery);
        if (roleResult.isEmpty()) {
            System.out.println("Error: Unable to retrieve user role.");
            return;
        }
        String userRole = roleResult.get(0).get(0).trim();

        String query;
        if (userRole.equals("Customer")) {
            query = String.format("SELECT orderID, storeID, totalPrice, orderTimestamp, orderStatus FROM FoodOrder WHERE login = '%s' ORDER BY orderTimestamp DESC;", authorisedUser);
        } else {
            query = "SELECT orderID, login, storeID, totalPrice, orderTimestamp, orderStatus FROM FoodOrder ORDER BY orderTimestamp DESC;";
        }

        List<List<String>> orders = esql.executeQueryAndReturnResult(query);
        System.out.printf("%-10s %-15s %-10s %-12s %-20s %-10s%n", "OrderID", "Username", "StoreID", "Total Price", "Timestamp", "Status");
        System.out.println("------------------------------------------------------------------------------------");
        for (List<String> row : orders) {
            System.out.printf("%-10s %-15s %-10s %-12s %-20s %-10s%n",
                row.get(0), (userRole.equals("Customer") ? authorisedUser : row.get(1)), row.get(2), row.get(3), row.get(4), row.get(5));
        }

    } catch (Exception e) {
        System.err.println("Error: " + e.getMessage());
    }
   }
   public static void viewRecentOrders(PizzaStore esql, String authorisedUser) {
      try {
        System.out.println("\n--- Recent 5 Orders ---");

        String query = String.format(
            "SELECT orderID, storeID, totalPrice, orderTimestamp, orderStatus " +
            "FROM FoodOrder WHERE login = '%s' ORDER BY orderTimestamp DESC LIMIT 5;", authorisedUser);

        List<List<String>> orders = esql.executeQueryAndReturnResult(query);
        System.out.printf("%-10s %-10s %-12s %-20s %-10s%n", "OrderID", "StoreID", "Total Price", "Timestamp", "Status");
        System.out.println("--------------------------------------------------------------------");
        for (List<String> row : orders) {
            System.out.printf("%-10s %-10s %-12s %-20s %-10s%n",
                row.get(0), row.get(1), row.get(2), row.get(3), row.get(4));
        }

      } catch (Exception e) {
        System.err.println("Error: " + e.getMessage());
      }
   }
   public static void viewOrderInfo(PizzaStore esql, String authorisedUser) {
      try {
        System.out.print("\nEnter Order ID to view details: ");
        int orderID = Integer.parseInt(in.readLine().trim());

        String roleQuery = String.format("SELECT TRIM(BOTH FROM role) FROM Users WHERE login = '%s';", authorisedUser);
        List<List<String>> roleResult = esql.executeQueryAndReturnResult(roleQuery);
        String userRole = roleResult.get(0).get(0).trim();

        String orderQuery = String.format(
            "SELECT login, storeID, totalPrice, orderTimestamp, orderStatus FROM FoodOrder WHERE orderID = %d;", orderID);
        List<List<String>> orderDetails = esql.executeQueryAndReturnResult(orderQuery);

        if (orderDetails.isEmpty()) {
            System.out.println("Error: Order not found.");
            return;
        }

        String orderUser = orderDetails.get(0).get(0);
        if (!orderUser.equals(authorisedUser) && !(userRole.equals("Manager") || userRole.equals("Driver"))) {
            System.out.println("Error: You do not have permission to view this order.");
            return;
        }

        System.out.printf("\nOrder ID: %d\nStore ID: %s\nTotal Price: %s\nTimestamp: %s\nStatus: %s\n\n",
            orderID, orderDetails.get(0).get(1), orderDetails.get(0).get(2), orderDetails.get(0).get(3), orderDetails.get(0).get(4));

        String itemsQuery = String.format("SELECT itemName, quantity FROM ItemsInOrder WHERE orderID = %d;", orderID);
        List<List<String>> items = esql.executeQueryAndReturnResult(itemsQuery);
        System.out.println("Items in Order:");
        for (List<String> item : items) {
            System.out.printf("- %s (x%s)\n", item.get(0), item.get(1));
        }

    } catch (Exception e) {
        System.err.println("Error: " + e.getMessage());
    }
   }
   public static void viewStores(PizzaStore esql) {
      try {
        System.out.println("\n--- List of Stores ---");

        String query = "SELECT storeID, address, city, state, isOpen, reviewScore FROM Store;";
        List<List<String>> stores = esql.executeQueryAndReturnResult(query);

         System.out.printf("%-10s %-30s %-15s %-20s %-10s %-10s%n", 
                          "Store ID", "Address", "City", "State", "Open", "Review");
        System.out.println("------------------------------------------------------------------------------------------------");

        for (List<String> row : stores) {
            System.out.printf("%-10s %-30s %-15s %-20s %-10s %-10s%n",
                row.get(0), row.get(1), row.get(2), row.get(3), row.get(4), row.get(5));
        }

      } catch (Exception e) {
        System.err.println("Error: " + e.getMessage());
      }
   }
   public static void updateOrderStatus(PizzaStore esql, String authorisedUser) {
      try {
        System.out.print("\nEnter Order ID to update status: ");
        int orderID = Integer.parseInt(in.readLine().trim());

        System.out.print("Enter new status (complete, incomplete, cancelled): ");
        String newStatus = in.readLine().trim();

        String roleQuery = String.format("SELECT TRIM(BOTH FROM role) FROM Users WHERE login = '%s';", authorisedUser);
        List<List<String>> roleResult = esql.executeQueryAndReturnResult(roleQuery);
        String userRole = roleResult.get(0).get(0).trim();

        if (!userRole.equalsIgnoreCase("Manager") && !userRole.equalsIgnoreCase("Driver")) {
            System.out.println("Error: Only Managers or Drivers can update order status.");
            return;
        }

        String updateQuery = String.format("UPDATE FoodOrder SET orderStatus = '%s' WHERE orderID = %d;", newStatus, orderID);
        esql.executeUpdate(updateQuery);
        System.out.println("Order status updated successfully!");

    } catch (Exception e) {
        System.err.println("Error: " + e.getMessage());
    }
   }
   public static void updateMenu(PizzaStore esql, String authorisedUser) {
          try {
        String roleQuery = String.format("SELECT TRIM(BOTH FROM role) FROM Users WHERE login = '%s';", authorisedUser);
        List<List<String>> roleResult = esql.executeQueryAndReturnResult(roleQuery);
        String userRole = roleResult.get(0).get(0).trim();
         if (!userRole.equalsIgnoreCase("Manager")) {
            System.out.println("Error: Only managers can update the menu.");
            return;
        }

        System.out.println("\n--- Update Menu ---");
        System.out.println("1. Update an existing item");
        System.out.println("2. Add a new item");
        System.out.print("Enter your choice: ");
        int choice = Integer.parseInt(in.readLine().trim());

        if (choice == 1) {
            System.out.print("Enter item name to update: ");
            String itemName = in.readLine().trim();

            System.out.println("Which field do you want to update?");
            System.out.println("1. Ingredients");
            System.out.println("2. Type");
            System.out.println("3. Price");
            System.out.println("4. Description");
            System.out.print("Enter your choice: ");
            int updateChoice = Integer.parseInt(in.readLine().trim());

            String updateQuery = "";
            switch (updateChoice) {
                case 1:
                    System.out.print("Enter new ingredients: ");
                    String newIngredients = in.readLine().trim();
                    updateQuery = String.format("UPDATE Items SET ingredients = '%s' WHERE itemName = '%s';", newIngredients, itemName);
                    break;
                case 2:
                    System.out.print("Enter new type (entree, drinks, sides): ");
                    String newType = in.readLine().trim();
                    updateQuery = String.format("UPDATE Items SET typeOfItem = '%s' WHERE itemName = '%s';", newType, itemName);
                    break;
                case 3:
                    System.out.print("Enter new price: ");
                    double newPrice = Double.parseDouble(in.readLine().trim());
                    updateQuery = String.format("UPDATE Items SET price = %.2f WHERE itemName = '%s';", newPrice, itemName);
                    break;
                case 4:
                    System.out.print("Enter new description: ");
                    String newDesc = in.readLine().trim();
                    updateQuery = String.format("UPDATE Items SET description = '%s' WHERE itemName = '%s';", newDesc, itemName);
                    break;
                default:
                    System.out.println("Invalid choice.");
                    return;
            }

            esql.executeUpdate(updateQuery);
            System.out.println("Menu item updated successfully!");

        } else if (choice == 2) {
            System.out.print("Enter new item name: ");
            String newItemName = in.readLine().trim();
            System.out.print("Enter ingredients: ");
            String ingredients = in.readLine().trim();
            System.out.print("Enter type (entree, drinks, sides): ");
            String type = in.readLine().trim();
            System.out.print("Enter price: ");
            double price = Double.parseDouble(in.readLine().trim());
            System.out.print("Enter description: ");
            String description = in.readLine().trim();

            String insertQuery = String.format(
                "INSERT INTO Items (itemName, ingredients, typeOfItem, price, description) VALUES ('%s', '%s', '%s', %.2f, '%s');",
                newItemName, ingredients, type, price, description);
            esql.executeUpdate(insertQuery);
            System.out.println("New menu item added successfully!");
        } else {
            System.out.println("Invalid choice.");
        }

    } catch (Exception e) {
        System.err.println("Error: " + e.getMessage());
    }
   }
   public static void updateUser(PizzaStore esql, String authorisedUser) {
      try {
        String roleQuery = String.format("SELECT TRIM(BOTH FROM role) FROM Users WHERE login = '%s';", authorisedUser);
        List<List<String>> roleResult = esql.executeQueryAndReturnResult(roleQuery);
        String userRole = roleResult.get(0).get(0).trim();
        if (!userRole.equalsIgnoreCase("Manager")) {
            System.out.println("Error: Only managers can update the menu.");
            return;
        }

        System.out.println("\n--- Update User ---");
        System.out.print("Enter the username of the user to update: ");
        String targetUser = in.readLine().trim();

        String checkQuery = String.format("SELECT COUNT(*) FROM Users WHERE login = '%s';", targetUser);
        List<List<String>> userCheckResult = esql.executeQueryAndReturnResult(checkQuery);
        if (Integer.parseInt(userCheckResult.get(0).get(0)) == 0) {
            System.out.println("Error: User not found.");
            return;
        }

        System.out.println("What would you like to update?");
        System.out.println("1. Change login username");
        System.out.println("2. Change password");
        System.out.println("3. Change role (Customer/Manager/Driver)");
        System.out.println("4. Update favorite items");
        System.out.println("5. Update phone number");
        System.out.print("Enter your choice: ");
        int choice = Integer.parseInt(in.readLine().trim());

        String updateQuery = "";
        switch (choice) {
            case 1:
                System.out.print("Enter new login username: ");
                String newLogin = in.readLine().trim();
                updateQuery = String.format("UPDATE Users SET login = '%s' WHERE login = '%s';", newLogin, targetUser);
                break;
            case 2:
                System.out.print("Enter new password: ");
                String newPassword = in.readLine().trim();
                updateQuery = String.format("UPDATE Users SET password = '%s' WHERE login = '%s';", newPassword, targetUser);
                break;
            case 3:
                System.out.print("Enter new role (Customer/Manager/Driver): ");
                String newRole = in.readLine().trim();
                updateQuery = String.format("UPDATE Users SET role = '%s' WHERE login = '%s';", newRole, targetUser);
                break;
            case 4:
                System.out.print("Enter new favorite items: ");
                String newFavItems = in.readLine().trim();
                updateQuery = String.format("UPDATE Users SET favoriteItems = '%s' WHERE login = '%s';", newFavItems, targetUser);
                break;
            case 5:
                System.out.print("Enter new phone number: ");
                String newPhone = in.readLine().trim();
                updateQuery = String.format("UPDATE Users SET phoneNum = '%s' WHERE login = '%s';", newPhone, targetUser);
                break;
            default:
                System.out.println("Invalid choice.");
                return;
        }

        esql.executeUpdate(updateQuery);
        System.out.println("User updated successfully!");

    } catch (Exception e) {
        System.err.println("Error: " + e.getMessage());
    }
   }


}//end PizzaStore

