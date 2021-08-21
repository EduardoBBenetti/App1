import java.util.*;
import java.io.*;
public class Main{
    public static void main(String[] args) {
        Scanner input = new Scanner(System.in);

        try {           

            //Reading the path of morse.txt file and storing the code in TreeMap.
            
            System.out.print("Enter Morse Code translation table file path: ");
            String morse_path = input.nextLine();

            File m_file = new File(morse_path); 
            BufferedReader m_br = new BufferedReader(new FileReader(m_file)); 
            TreeMap<String,String> morse = new TreeMap<String,String>();

            String st; 
            int loaded = 0;

            while ((st = m_br.readLine()) != null) {
                morse.put(st.split(" ")[1],st.split(" ")[0]);
                loaded++;
            }

            System.out.println("Code translation file processed. "+loaded+" codes loaded.\n");

            //Reading the coded message file and storing the message in arraylist.
            System.out.print("Enter coded message input file path: ");
            String coded = input.nextLine();

            File c_file = new File(coded); 
            BufferedReader c_br = new BufferedReader(new FileReader(c_file)); 

            ArrayList<String[]> coded_message = new ArrayList<String[]>();

            String str;

            while ((str = c_br.readLine()) != null ){
                coded_message.add(str.split(" "));
            }

            //Forming the decode message file.
            System.out.print("Enter decoded message input file path: ");
            String decoded = input.nextLine();

            FileWriter myWriter = new FileWriter(decoded);

            String final_string = "";
            int number = 0;

            for( int i = 0; i < coded_message.size(); i++) {

                for(int j = 0; j < coded_message.get(i).length; j++) {
                     final_string += morse.get(coded_message.get(i)[j]);               
                }

                if(final_string.charAt(final_string.length() -1) == '.') {
                    final_string +="\n";
                }

                else {
                    final_string +=" ";
                }

                number++;
            }

            myWriter.write(final_string);
            myWriter.close();

            System.out.println("Messsage translation complete. "+number+" words processed.");

        } catch (Exception e) {
            System.out.println("File not found");
        } 
    }
}