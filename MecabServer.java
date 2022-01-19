import java.io.*;
import java.net.*;

public class MecabServer {
    int port;
    ServerSocket socket;

    public static void main(String[] args) {
        try {
            new MecabServer(8888).start();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public MecabServer(int port) throws IOException {
        this.port = port;
        init();
    }

    private void init() throws IOException {
        socket = new ServerSocket(port);
    }

    private void start() {
        Socket connection = null;
        while(true){
            try {
                connection = socket.accept();  
                new Thread(new RequestProcessor(connection)).start(); 
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private class RequestProcessor implements Runnable {
        private Socket connection;
        private BufferedReader sockIn, processIn;
        private PrintWriter sockOut, processOut;
        Process mecab;

        RequestProcessor(Socket connection) throws IOException {
            this.connection = connection;
            sockIn = new BufferedReader(new InputStreamReader(this.connection.getInputStream()));
            sockOut = new PrintWriter(this.connection.getOutputStream());
            mecab = Runtime.getRuntime().exec("mecab");
            processIn = new BufferedReader(new InputStreamReader(mecab.getInputStream()));
            processOut = new PrintWriter(mecab.getOutputStream());
        }

        @Override
        public void run() {
            String UserLine, processLine;
            try {
                while (true) {
                    UserLine = sockIn.readLine();
                    if (UserLine == null) {
                        break;
                    }
                    processOut.println(UserLine);
                    processOut.flush();
                    while (!(processLine = processIn.readLine()).equals("")) {
                        System.out.println(processLine);
                        sockOut.println(processLine);
                        sockOut.flush();
                        if(processLine.equals("EOS")){
                            break;
                        }
                    }
                    
                    System.out.println("next");
                }
            } catch (IOException e) {
                e.printStackTrace();
            } finally {
                try {
                    sockIn.close();
                    sockOut.close();
                    processIn.close();
                    processIn.close();
                    mecab.destroy();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }
}