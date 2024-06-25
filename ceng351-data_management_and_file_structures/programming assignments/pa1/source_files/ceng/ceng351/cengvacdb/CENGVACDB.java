package ceng.ceng351.cengvacdb;

import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

public class CENGVACDB implements ICENGVACDB{
    private static String user = "e2380947"; // TODO: Your userName
    private static String password = "Hx8GfzVbS4Ra"; //  TODO: Your password
    private static String host = "144.122.71.121"; // host name
    private static String database = "db2380947"; // TODO: Your database name
    private static int port = 8080; // port

    private static Connection connection = null;


    @Override
    public void initialize() {
        String url = "jdbc:mysql://" + host + ":" + port + "/" + database + "?useSSL=false";
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            connection =  DriverManager.getConnection(url, user, password);
        }
        catch (SQLException | ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    @Override
    public int createTables() {
        int result;
        int numberofTablesInserted = 0;

        String queryCreateUserTable = "create table User (" +
                "userID int not null," +
                "userName varchar(30)," +
                "age INT," +
                "address varchar(150)," +
                "password varchar(30)," +
                "status varchar(15)," +
                "primary key (userID))";

        String queryCreateVaccineTable = "create table Vaccine (" +
                "code int not null," +
                "vaccinename varchar(30)," +
                "type varchar(30)," +
                "primary key (code))";

        String queryCreateVaccinationTable = "create table Vaccination (" +
                "code int not null," +
                "userID int not null," +
                "dose int," +
                "vacdate date," +
                "primary key (code,userID,dose),"+
                "foreign key (code) references Vaccine(code) on delete cascade,"+
                "foreign key (userID) references User(userID)) ";

        String queryCreateAllergicSideEffectsTable = "create table AllergicSideEffect (" +
                "effectcode int not null," +
                "effectname varchar(50)," +
                "primary key (effectcode))";

        String queryCreateSeenTable = "create table Seen (" +
                "effectcode int not null," +
                "code int not null," +
                "userID int not null," +
                "date date," +
                "degree varchar(30)," +
                "primary key (effectcode,code,userID),"+
                "foreign key (effectcode) references AllergicSideEffect(effectcode) on delete cascade ,"+
                "foreign key (code) references Vaccination(code) on delete cascade,"+
                "foreign key (userID) references User(userID)) ";
        try {
            Statement statement = connection.createStatement();
            //User Table
            statement.executeUpdate(queryCreateUserTable);
            numberofTablesInserted++;
            //close
            statement.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        try {
            Statement statement = connection.createStatement();
            //Vaccine Table
            statement.executeUpdate(queryCreateVaccineTable);
            numberofTablesInserted++;
            //close
            statement.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        try {
            Statement statement = connection.createStatement();
            //Vaccination Table
            statement.executeUpdate(queryCreateVaccinationTable);
            numberofTablesInserted++;
            //close
            statement.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        try {
            Statement statement = connection.createStatement();
            //AllergicSideEffect Table
            statement.executeUpdate(queryCreateAllergicSideEffectsTable);
            numberofTablesInserted++;
            //close
            statement.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        try {
            Statement statement = connection.createStatement();
            //Seen Table
            statement.executeUpdate(queryCreateSeenTable);
            numberofTablesInserted++;
            //close
            statement.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return numberofTablesInserted;
    }

    @Override
    public int dropTables() {
        int numberofTablesDropped = 0;
        String queryDropUserTable = "drop table if exists User";
        String queryDropVaccineTable = "drop table if exists Vaccine";
        String queryDropVaccinationTable = "drop table if exists Vaccination";
        String queryDropAllergicSideEffectTable = "drop table if exists AllergicSideEffect";
        String queryDropSeenTable = "drop table if exists Seen";

        try {
            Statement statement = connection.createStatement();
            //Seen
            statement.executeUpdate(queryDropSeenTable);
            numberofTablesDropped++;
            //close
            statement.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        try {
            Statement statement = connection.createStatement();
            //AllergicSideEffect
            statement.executeUpdate(queryDropAllergicSideEffectTable);
            numberofTablesDropped++;
            //close
            statement.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        try {
            Statement statement = connection.createStatement();
            //Vaccination
            statement.executeUpdate(queryDropVaccinationTable);
            numberofTablesDropped++;
            //close
            statement.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        try {
            Statement statement = connection.createStatement();
            //User
            statement.executeUpdate(queryDropUserTable);
            numberofTablesDropped++;
            //close
            statement.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        try {
            Statement statement = connection.createStatement();
            //Vaccine
            statement.executeUpdate(queryDropVaccineTable);
            numberofTablesDropped++;
            //close
            statement.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return numberofTablesDropped ;
    }

    @Override
    public int insertUser(User[] users) {
        int numberofRowsInserted = 0;
        int result = 0;
        String query;
        for (int i = 0; i < users.length; i++) {
            query = "insert into User values ('" +
                    users[i].getUserID()+ "','" +
                    users[i].getUserName() + "','" +
                    users[i].getAge() + "','" +
                    users[i].getAddress() + "','" +
                    users[i].getPassword() + "','" +
                    users[i].getStatus() + "')" ;
            try {
                Statement st = this.connection.createStatement();
                result = st.executeUpdate(query);
                //Close
                st.close();
                numberofRowsInserted++;
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
        return numberofRowsInserted;
    }

    @Override
    public int insertAllergicSideEffect(AllergicSideEffect[] sideEffects) {
        int numberofRowsInserted = 0;
        int result = 0;
        String query;
        for (int i = 0; i < sideEffects.length; i++) {
            query = "insert into AllergicSideEffect values ('" +
                    sideEffects[i].getEffectCode()+ "','" +
                    sideEffects[i].getEffectName() + "')" ;
            try {
                Statement st = this.connection.createStatement();
                result = st.executeUpdate(query);
                //Close
                st.close();
                numberofRowsInserted++;
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
        return numberofRowsInserted;
    }

    @Override
    public int insertVaccine(Vaccine[] vaccines) {
        int numberofRowsInserted = 0;
        int result = 0;
        String query;
        for (int i = 0; i < vaccines.length; i++) {
            query = "insert into Vaccine values ('" +
                    vaccines[i].getCode()+ "','" +
                    vaccines[i].getVaccineName()+ "','" +
                    vaccines[i].getType() + "')" ;
            try {
                Statement st = this.connection.createStatement();
                result = st.executeUpdate(query);
                //Close
                st.close();
                numberofRowsInserted++;
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
        return numberofRowsInserted;
    }

    @Override
    public int insertVaccination(Vaccination[] vaccinations) {
        int numberofRowsInserted = 0;
        int result = 0;
        String query;
        for (int i = 0; i < vaccinations.length; i++) {
            query = "insert into Vaccination values ('" +
                    vaccinations[i].getCode()+ "','" +
                    vaccinations[i].getUserID()+ "','" +
                    vaccinations[i].getDose()+ "','" +
                    vaccinations[i].getVacdate() + "')" ;
            try {
                Statement st = this.connection.createStatement();
                result = st.executeUpdate(query);
                //Close
                st.close();
                numberofRowsInserted++;
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
        return numberofRowsInserted;
    }

    @Override
    public int insertSeen(Seen[] seens) {
        int numberofRowsInserted = 0;
        int result = 0;
        String query;
        for (int i = 0; i < seens.length; i++) {
            query = "insert into Seen values ('" +
                    seens[i].getEffectcode()+ "','" +
                    seens[i].getCode()+ "','" +
                    seens[i].getUserID()+ "','" +
                    seens[i].getDate()+ "','" +
                    seens[i].getDegree() + "')" ;
            try {
                Statement st = this.connection.createStatement();
                result = st.executeUpdate(query);
                //Close
                st.close();
                numberofRowsInserted++;
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
        return numberofRowsInserted;
    }

    @Override
    public Vaccine[] getVaccinesNotAppliedAnyUser() {
        ArrayList<Vaccine> rlist = new ArrayList<Vaccine>();
        Vaccine r[];
        Vaccine r_new;
        ResultSet rs ;
        String query=   "select distinct v.code, v.vaccinename, v.type " +
                "from Vaccine v " +
                "WHERE v.code NOT IN " +
                "(Select v2.code " +
                "from Vaccination v2) " +
                "Order by v.code asc;";
        try {
            Statement st = connection.createStatement();
            rs = st.executeQuery(query);
            while( rs.next() ) {
                Integer r_code= rs.getInt("code");
                String r_vaccinename = rs.getString("vaccinename");
                String r_type = rs.getString("type");
                r_new = new Vaccine(r_code, r_vaccinename,r_type);
                rlist.add(r_new) ;
            }
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        r = new Vaccine[rlist.size()];
        r = rlist.toArray(r);
        return r;
    }

    @Override
    public QueryResult.UserIDuserNameAddressResult[] getVaccinatedUsersforTwoDosesByDate(String vacdate) {
        ResultSet rs;
        List<QueryResult.UserIDuserNameAddressResult> ansngrList = new ArrayList<QueryResult.UserIDuserNameAddressResult>();

        String query=   " select DISTINCT u.userID, u.userName, u.address " +
                "from User u " +
                "where u.userID "+
                "IN " +
                " (select v.userID " +
                "from Vaccination v " +
                "where v.vacdate > '" + vacdate + "' and v.dose=1  and v.userID " +
                "IN "+
                " (select v2.userID " +
                " from Vaccination v2 " +
                " where v2.vacdate > '"+ vacdate + "' and v2.dose=2)" +
                " and v.userID " +
                " NOT IN " +
                " (select distinct v3.userID "+
                " from Vaccination v3 " +
                " where v3.dose = 3 and v3.vacdate > '" +  vacdate + "') )" +
                " order by u.userID asc; ";
        try {
            Statement st = connection.createStatement();
            rs = st.executeQuery(query);
            while( rs.next() ) {
                QueryResult.UserIDuserNameAddressResult ansngr = null;

                String r_userID= String.valueOf(rs.getInt("userID"));
                String r_userName = rs.getString("userName");
                String r_address = rs.getString("address");
                ansngr = new QueryResult.UserIDuserNameAddressResult(r_userID, r_userName,r_address);
                ansngrList.add(ansngr);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        QueryResult.UserIDuserNameAddressResult[] ansngrArray = new QueryResult.UserIDuserNameAddressResult[ansngrList.size()];
        ansngrArray = ansngrList.toArray(ansngrArray);
        return ansngrArray;
    }

    @Override
    public Vaccine[] getTwoRecentVaccinesDoNotContainVac() {
        ResultSet rs;
        List<Vaccine> ansngrList = new ArrayList<Vaccine>();

        String query=   "select distinct vv.code, vv.vaccinename, vv.type " +
                "from Vaccine vv " +
                "where vv.code IN " +
                "(select TEMP2.code from( select distinct v2.code,max( v2.vacdate) as mdate from Vaccination v2 where v2.code IN  (select v.code  from Vaccine v where v.vaccinename not like '%vac%')  and v2.code NOT IN (select TEMP1.code from  (select distinct v2.code,max( v2.vacdate) as mdate from Vaccination v2 where v2.code IN  (select v.code  from Vaccine v where v.vaccinename not like '%vac%')  Group by v2.code ) as TEMP1 Where TEMP1.mdate =  ( select max( v2.vacdate) from Vaccination v2 where v2.code IN  (select v.code  from Vaccine v where v.vaccinename not like '%vac%')  )) Group by v2.code ) as TEMP2 Where TEMP2.mdate=  (select max(TMM.mdate) from (select distinct max( v2.vacdate) as mdate from Vaccination v2 where v2.code IN  (select v.code  from Vaccine v where v.vaccinename not like '%vac%')  and v2.code NOT IN (select TEMP1.code from  (select distinct v2.code,max( v2.vacdate) as mdate from Vaccination v2 where v2.code IN  (select v.code  from Vaccine v where v.vaccinename not like '%vac%')  Group by v2.code ) as TEMP1 Where TEMP1.mdate = ( select max( v2.vacdate) from Vaccination v2 where v2.code IN  (select v.code  from Vaccine v where v.vaccinename not like '%vac%') )) Group by v2.code) as TMM) ) " +
                "or vv. code IN " +
                "(select TEMP1.code from  (select distinct v2.code,max( v2.vacdate) as mdate from Vaccination v2 where v2.code IN (select v.code  from Vaccine v where v.vaccinename not like '%vac%')  Group by v2.code ) as TEMP1 Where TEMP1.mdate =  ( select max( v2.vacdate) from Vaccination v2 where v2.code IN  (select v.code  from Vaccine v where v.vaccinename not like '%vac%') )) " +
                "order by vv.code asc; ";
        try {
            Statement st = connection.createStatement();
            rs = st.executeQuery(query);
            while( rs.next() ) {
                Vaccine ansngr = null;
                Integer r_code= rs.getInt("code");
                String r_vaccinename = rs.getString("vaccinename");
                String r_type = rs.getString("type");
                ansngr = new Vaccine(r_code, r_vaccinename,r_type);
                ansngrList.add(ansngr);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        Vaccine[] ansngrArray = new Vaccine[ansngrList.size()];
        ansngrArray = ansngrList.toArray(ansngrArray);
        return ansngrArray;
    }

    @Override
    public QueryResult.UserIDuserNameAddressResult[] getUsersAtHasLeastTwoDoseAtMostOneSideEffect() {
        ResultSet rs;
        List<QueryResult.UserIDuserNameAddressResult> ansngrList = new ArrayList<QueryResult.UserIDuserNameAddressResult>();

        String query=   "select distinct u.userID, u.username, u.address from User u where u.userID IN " +
                "( select distinct v.userID  from Vaccination v where v.dose>1   and v.userID NOT IN ( select s.userId from Seen s group by s.userID having count(*)>1 ) ) " +
                "order by u.userID asc;";
        try {
            Statement st = connection.createStatement();
            rs = st.executeQuery(query);
            while( rs.next() ) {
                QueryResult.UserIDuserNameAddressResult ansngr = null;
                String r_userID= String.valueOf(rs.getInt("userID"));
                String r_username = rs.getString("username");
                String r_address = rs.getString("address");
                ansngr = new QueryResult.UserIDuserNameAddressResult(r_userID, r_username,r_address);
                ansngrList.add(ansngr);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        QueryResult.UserIDuserNameAddressResult[] ansngrArray = new QueryResult.UserIDuserNameAddressResult[ansngrList.size()];
        ansngrArray = ansngrList.toArray(ansngrArray);
        return ansngrArray;
    }

    @Override
    public QueryResult.UserIDuserNameAddressResult[] getVaccinatedUsersWithAllVaccinesCanCauseGivenSideEffect(String effectname) {
        ResultSet rs;
        List<QueryResult.UserIDuserNameAddressResult> ansngrList = new ArrayList<QueryResult.UserIDuserNameAddressResult>();

        String query=   "select distinct u.userID, u.username, u.address from User u where u.userID IN " +
                "(select  s.userID from Seen s, AllergicSideEffect a where a.effectcode = s.effectcode and a.effectname =  \"" + effectname + "\") " +
                "order by u.userID asc;";
        try {
            Statement st = connection.createStatement();
            rs = st.executeQuery(query);
            while( rs.next() ) {
                QueryResult.UserIDuserNameAddressResult ansngr = null;
                String r_userID= String.valueOf(rs.getInt("userID"));
                String r_username = rs.getString("username");
                String r_address = rs.getString("address");
                ansngr = new QueryResult.UserIDuserNameAddressResult(r_userID, r_username,r_address);
                ansngrList.add(ansngr);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        QueryResult.UserIDuserNameAddressResult[] ansngrArray = new QueryResult.UserIDuserNameAddressResult[ansngrList.size()];
        ansngrArray = ansngrList.toArray(ansngrArray);
        return ansngrArray;
    }

    @Override
    public QueryResult.UserIDuserNameAddressResult[] getUsersWithAtLeastTwoDifferentVaccineTypeByGivenInterval(String startdate, String enddate) {
        ResultSet rs;
        List<QueryResult.UserIDuserNameAddressResult> ansngrList = new ArrayList<QueryResult.UserIDuserNameAddressResult>();

        String query=   "select distinct u.userID, u.username, u.address from User u where u.userID IN ( " +
                "select v.userID from Vaccination v, Vaccination v2 where v.userID = v2.userID and v.code<>v2.code and " +
                "'"+startdate+"' <= v.vacdate and v.vacdate<='"+enddate+"' and " +
                "'"+startdate+"' <= v2.vacdate and v2.vacdate<='"+enddate+"' );";
        try {
            Statement st = connection.createStatement();
            rs = st.executeQuery(query);
            while( rs.next() ) {
                QueryResult.UserIDuserNameAddressResult ansngr = null;
                String r_userID= String.valueOf(rs.getInt("userID"));
                String r_username = rs.getString("username");
                String r_address = rs.getString("address");
                ansngr = new QueryResult.UserIDuserNameAddressResult(r_userID, r_username,r_address);
                ansngrList.add(ansngr);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        QueryResult.UserIDuserNameAddressResult[] ansngrArray = new QueryResult.UserIDuserNameAddressResult[ansngrList.size()];
        ansngrArray = ansngrList.toArray(ansngrArray);
        return ansngrArray;
    }

    @Override
    public AllergicSideEffect[] getSideEffectsOfUserWhoHaveTwoDosesInLessThanTwentyDays() {
        ResultSet rs;
        List<AllergicSideEffect> ansngrList = new ArrayList<AllergicSideEffect>();
        String query=   "select distinct a.effectcode, a.effectname from AllergicSideEffect a where a.effectcode IN (select s.effectcode from Seen s where (s.code, s.userID) IN (select v.code, v.userID from Vaccination v, Vaccination v2 where v.userID = v2.userID and v.dose<>v2.dose and  DATEDIFF(v2.vacdate, v.vacdate) <20 and DATEDIFF(v2.vacdate, v.vacdate)>-20 ) ) order by a.effectcode asc; ;";
        try {
            Statement st = connection.createStatement();
            rs = st.executeQuery(query);
            while( rs.next() ) {
                AllergicSideEffect ansngr = null;
                Integer r_effectcode= rs.getInt("effectcode");
                String r_effectname = rs.getString("effectname");
                ansngr = new AllergicSideEffect(r_effectcode,r_effectname);
                ansngrList.add(ansngr);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        AllergicSideEffect[] ansngrArray = new AllergicSideEffect[ansngrList.size()];
        ansngrArray = ansngrList.toArray(ansngrArray);
        return ansngrArray;
    }

    @Override
    public double averageNumberofDosesofVaccinatedUserOverSixtyFiveYearsOld() {
        ResultSet rs;
        double res = 0;

        String querySum = "select AVG(TMP.mdos) as av from (select  MAX(v.dose) as mdos from User u, Vaccination v where u.userID=v.userID and u.age>65 group by u.userID) as TMP ;";

        try {
            Statement st = this.connection.createStatement();
            rs = st.executeQuery(querySum);
            rs.next();
            res = rs.getDouble("av");
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        return res;
    }

    @Override
    public int updateStatusToEligible(String givendate) {
        int result = 0;
        String query ="update User u " +
                "set u.status= 'Eligible' " +
                "where u.userID IN " +
                "(select v.userID from Vaccination v group by v.userID having datediff('"+ givendate +"' ,MAX(v.vacdate) )>120) " +
                "and u.status= 'Not_Eligible';";
        try {
            Statement st  = this.connection.createStatement();
            result = st.executeUpdate(query);
        }catch (SQLException e) {
            e.printStackTrace();
        }
        return result;
    }

    @Override
    public Vaccine deleteVaccine(String vaccineName) {
        Vaccine vac = null ;
        ResultSet rs ;
        String query = "select * from Vaccine v "+
                "where v.vaccinename = \"" +
                vaccineName +
                "\";" ;
        try {
            Statement st = connection.createStatement();
            rs = st.executeQuery(query);
            rs.next();

            Integer r_code= rs.getInt("code");
            String r_vaccinename = rs.getString("vaccinename");
            String r_type = rs.getString("type");
            vac = new Vaccine(r_code,r_vaccinename,r_type);
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }

        query = "delete from Vaccine v "+
                "where v.vaccinename ='"+vaccineName+"' ;" ;

        try {
            Statement st = connection.createStatement();
            st.executeUpdate(query);
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return vac;

    }
}
