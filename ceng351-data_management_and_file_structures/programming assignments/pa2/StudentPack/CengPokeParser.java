import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;

public class CengPokeParser {

	public static ArrayList<CengPoke> parsePokeFile(String filename)
	{
		ArrayList<CengPoke> pokeList = new ArrayList<CengPoke>();

		// You need to parse the input file in order to use GUI tables.
		// TODO: Parse the input file, and convert them into CengPokes

		return pokeList;
	}
	
	public static void startParsingCommandLine() throws IOException
	{
		BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
		boolean running = true;
		String inpLine = null;
		while(running){

			inpLine = reader.readLine();
			String[] array = inpLine.split("\\t");
			String command = array[0];
			if(command.equalsIgnoreCase("add")){
				CengPokeKeeper.addPoke(new CengPoke(Integer.parseInt(array[1]),array[2],array[3],array[4]));
			}

			else if(command.equalsIgnoreCase("quit")){
				running = false;
			}
			else if(command.equalsIgnoreCase("search")){
				int parsedKey = Integer.parseInt(array[1]);
				CengPokeKeeper.searchPoke(parsedKey);
			}
			else if(command.equalsIgnoreCase("delete")){
				int parsedKey = Integer.parseInt(array[1]);
				CengPokeKeeper.deletePoke(parsedKey);
			}
			else if(command.equalsIgnoreCase("print")){
				CengPokeKeeper.printEverything();
			}
		}
	}
}
