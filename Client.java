import java.io.*;
import java.net.*;
public class Client {
    public static void main(String[] args) throws UnknownHostException, IOException {
        BufferedReader sockio, stdio = new BufferedReader(new InputStreamReader(System.in));
        String line, returnString;
        Socket connection;
        PrintWriter pw;
        System.out.println("host:");
        connection = new Socket((stdio.readLine()), 8888);
        sockio = new BufferedReader(new InputStreamReader(connection.getInputStream()));
        pw = new PrintWriter(connection.getOutputStream());
        while(true){
            line = stdio.readLine();
            if(line.equals("")){
                break;
            }
            pw.println(line);
            pw.flush();
            while(true){
                returnString = sockio.readLine();
                if(returnString.equals("EOS")){
                    break;
                }
                System.out.println(returnString);
            }
           
        }
        pw.print("");
        pw.flush();
        pw.close();
        stdio.close();
        connection.close();
        sockio.close();
    }
}
