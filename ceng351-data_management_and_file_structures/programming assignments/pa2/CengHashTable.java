import java.util.ArrayList;

public class CengHashTable {
	private int bucketSize;
	private int globalDepth = 0;
	private ArrayList<CengBucket> table;


	public CengHashTable()
	{
		this.bucketSize=CengPokeKeeper.getBucketSize();
		this.globalDepth = 0;
		this.table = new ArrayList<CengBucket>(1);
		CengBucket bucket0 = new CengBucket(bucketSize);
		this.table.add(bucket0);
	}

	public void deletePoke(Integer pokeKey)
	{
		int wh=calculateHash(pokeKey);
		CengBucket t = this.table.get(wh);
		t.delete(pokeKey);
		int empty_bu=0;
		int dif;

		for(int i =0;i<this.table.size()-1;i++){
			if(this.table.get(i).getLoadFactor()==0) {
				if(this.table.get(i)!=this.table.get(i+1)){
					empty_bu++;
				}
			}
		}
		if(this.table.size()>1){
			if(this.table.get(this.table.size()-1).getLoadFactor()==0){
				empty_bu++;
			}
		}



		System.out.println("\"delete\": {");
		System.out.println("\t\"emptyBucketNum\": "+empty_bu);
		System.out.println("}");

	}

	public void addPoke(CengPoke poke)
	{
		int key= poke.pokeKey();
		int hash = calculateHash(key);
		CengBucket proposed = this.table.get(hash);

		if(proposed.find(poke)==-1){
			if (proposed.getLoadFactor() < this.bucketSize) {
				try {
					proposed.insert(poke);
				} catch (Exception e) {
					System.out.println("Exception");
				}
			}
			else {//split
				if (proposed.getLocalDepth() == this.globalDepth) {
					increaseGlobalDepth();
				}
				splitBucket(key, proposed.getLocalDepth(), this.globalDepth);
				addPoke(poke);
			}
		}
		 else {}

	}

	private void splitBucket(int key, int localDepth, int oldGlobalDepth) {
		int us = oldGlobalDepth-localDepth;
		int bias = 1 << (us);
		CengBucket nbucket0 = new CengBucket(this.bucketSize);
		CengBucket nbucket1 = new CengBucket(this.bucketSize);

		nbucket0.setLocalDepth(localDepth + 1);
		nbucket1.setLocalDepth(localDepth + 1);

		CengBucket oldBin = this.table.get(calculateHash(key));
		CengPoke[] elements = oldBin.getBucketElements();


		for (int i = 0; i < this.bucketSize; i++) {

			CengPoke t = elements[i];
			int st= t.pokeKey();

			int hash_mod0=CengPokeKeeper.getHashMod();
			int n0 = st % hash_mod0;
			int digits0 = (int)(Math.log(hash_mod0) / Math.log(2));
			String s0 = "";
			while (n0 > 0){
				s0 =  ( (n0 % 2 ) == 0 ? "0" : "1") +s0;
				n0 = n0 / 2;
			}

			int zeros0;
			zeros0=digits0-s0.length();
			while (zeros0 > 0){
				s0="0"+s0;
				zeros0--;
			}

			String hash_val0 = "";
			hash_val0 = s0.substring(0, localDepth + 1);
			String binary0 = hash_val0;
			char[] numbers0 = binary0.toCharArray();
			int result0 = 0;
			for(int i0=numbers0.length - 1; i0>=0; i0--)
				if(numbers0[i0]=='1')
					result0 += Math.pow(2, (numbers0.length-i0 - 1));

			if (result0 %2==0) {
				try {
					nbucket0.insert(t);
				} catch (Exception e) {
					System.out.println("Exception");
				}
			} else {
				try {
					nbucket1.insert(t);
				} catch (Exception e) {
					System.out.println("Exception");
				}
			}
		}

		int hash_mod=CengPokeKeeper.getHashMod();
		int n = key % hash_mod;

		int digits = (int)(Math.log(hash_mod) / Math.log(2));
		String s = "";
		while (n > 0){
			s =  ( (n % 2 ) == 0 ? "0" : "1") +s;
			n = n / 2;
		}
		int zeros;
		zeros=digits-s.length();
		while (zeros > 0){
			s="0"+s;
			zeros--;
		}
		String hash_val = "";
		hash_val = s.substring(0, globalDepth-us);
		zeros=us;
		while (zeros > 0){
			hash_val=hash_val+"0";
			zeros--;
		}

		String binary = hash_val;
		char[] numbers = binary.toCharArray();
		int baslangic = 0;
		for(int i=numbers.length - 1; i>=0; i--)
			if(numbers[i]=='1')
				baslangic += Math.pow(2, (numbers.length-i - 1));



		for (int i = 0;i<(bias/2);i++){
			this.table.set(baslangic+i, nbucket0);
		}
		int ikincibas=baslangic+(bias/2);
		for (int i = 0;i<(bias/2);i++){
			this.table.set(ikincibas+i, nbucket1);
		}



	}

	
	public void searchPoke(Integer pokeKey)
	{
		System.out.println("\"search\": {");
		int i = calculateHash(pokeKey);
		boolean exist=false;
		CengBucket oldBin = this.table.get(i);
		int tt=globalDepth-oldBin.getLocalDepth();

		int us = globalDepth-oldBin.getLocalDepth();
		int hash_mod=CengPokeKeeper.getHashMod();
		int n = pokeKey % hash_mod;
		int digits = (int)(Math.log(hash_mod) / Math.log(2));
		String s = "";
		while (n > 0){
			s =  ( (n % 2 ) == 0 ? "0" : "1") +s;
			n = n / 2;
		}
		int zeros;
		zeros=digits-s.length();
		while (zeros > 0){
			s="0"+s;
			zeros--;
		}
		String hash_val = "";
		hash_val = s.substring(0, globalDepth-us);
		zeros=us;
		while (zeros > 0){
			hash_val=hash_val+"0";
			zeros--;
		}

		String binary = hash_val;
		char[] numbers = binary.toCharArray();
		int baslangic = 0;
		for(int j=numbers.length - 1; j>=0; j--)
			if(numbers[j]=='1')
				baslangic += Math.pow(2, (numbers.length-j - 1));
		i=baslangic;




		tt=1<<tt;
		int a = oldBin.getLoadFactor();
		for(int j=0;j<a;j++){
			if(oldBin.getBucketElements()[j].pokeKey().equals(pokeKey))
				exist=true;
		}
		if(exist) {
			while (tt > 0) {
				System.out.println("\t\"row\": {");
				System.out.println("\t\t\"hashPref\": " + convertToBinary(i) + ",");
				System.out.println("\t\t\"bucket\": {");
				System.out.println("\t\t\t\"hashLength\": " + this.table.get(i).getLocalDepth() + ",");
				System.out.println("\t\t\t\"pokes\": [");
				System.out.println(this.table.get(i).getBinContents());
				System.out.println("\t\t\t]");
				System.out.println("\t\t}");
				if((tt-1)!=0)
				System.out.println("\t},");
				else
					System.out.println("\t}");
				tt--;
				if(tt>0){
					i++;
					oldBin = this.table.get(i);
				}

			}
		}


		System.out.println("}");
	}
	
	public void print()
	{
		System.out.println("\"table\": {");
		for(int i=0; i < this.table.size(); i++){
			System.out.println("\t\"row\": {");
			System.out.println("\t\t\"hashPref\": "+convertToBinary(i)+",");
			System.out.println("\t\t\"bucket\": {");
			System.out.println("\t\t\t\"hashLength\": "+this.table.get(i).getLocalDepth()+",");
			System.out.println("\t\t\t\"pokes\": [");
			if(this.table.get(i).getBinContents()!="")
			{System.out.println(this.table.get(i).getBinContents());}
			System.out.println("\t\t\t]");
			System.out.println("\t\t}");
			if(i==this.table.size()-1){
				System.out.println("\t}");
			}
			else{
				System.out.println("\t},");
			}
		}
		System.out.println("}");


	}
	private String convertToBinary(int val) {
		int l = this.globalDepth;
		String r = Integer.toBinaryString(val);
		int m = r.length();
		String rr = "";
		if (m < l) {
			for (int i = 0; i < (l-m); i++)
				rr += "0";
		}
		rr += r;
		return rr;
	}

	protected int calculateHash(int val) {
		int hash_mod=CengPokeKeeper.getHashMod();
		int n = val % hash_mod;
		int digits = (int)(Math.log(hash_mod) / Math.log(2));
		String s = "";
		while (n > 0){
			s =  ( (n % 2 ) == 0 ? "0" : "1") +s;
			n = n / 2;
		}
		int zeros;
		zeros=digits-s.length();
		while (zeros > 0){
			s="0"+s;
			zeros--;
		}
		String hash_val = "";
		hash_val = s.substring(0, globalDepth);
		String binary = hash_val;
		char[] numbers = binary.toCharArray();
		int result = 0;
		for(int i=numbers.length - 1; i>=0; i--)
			if(numbers[i]=='1')
				result += Math.pow(2, (numbers.length-i - 1));
		return  result;

	}

	private void increaseGlobalDepth() {
		int oldsize=this.table.size();
		ArrayList<CengBucket> new_table=new ArrayList<CengBucket>(2*oldsize);
		for(int i=0;i<oldsize;i++){
			new_table.add(this.table.get(i));
			new_table.add(this.table.get(i));
		}
		this.table.clear();
		for(int i=0;i<2*oldsize;i++) {
			this.table.add(new_table.get(i));
		}
		new_table.clear();
		this.globalDepth++;
	}


	// GUI-Based Methods
	// These methods are required by GUI to work properly.
	
	public int prefixBitCount()
	{
		// TODO: Return table's hash prefix length.
		return 0;		
	}
	
	public int rowCount()
	{
		// TODO: Return the count of HashRows in table.
		return 0;		
	}
	
	public CengHashRow rowAtIndex(int index)
	{
		// TODO: Return corresponding hashRow at index.
		return null;
	}
	
	// Own Methods
}
