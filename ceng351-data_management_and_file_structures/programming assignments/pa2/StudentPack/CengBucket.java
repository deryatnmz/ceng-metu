public class CengBucket {

	private int bucketSize;
	private int localDepth = 0;
	private CengPoke[] buckets;
	private int loadFactor = 0;
	public CengBucket(int bucketSize) {
		this.bucketSize = bucketSize;
		this.localDepth = 0;
		this.buckets = new CengPoke[bucketSize];
	}
	public int getBucketSize() {
		return this.bucketSize;
	}
	public int getLoadFactor() {
		return this.loadFactor;
	}
	public int getLocalDepth() {
		return this.localDepth;
	}
	public void setLocalDepth(int ld) {
		this.localDepth = ld;
	}

	public  String hhash(int val){
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
		return s;
	}

	public String getBinContents() {
		String line = "";
		for (int i = 0; i < this.loadFactor; i++) {
			line +="\t\t\t\t\"poke\": {\n\t\t\t\t\t\"hash\": "+hhash(this.buckets[i].pokeKey())+",\n\t\t\t\t\t\"pokeKey\": "+this.buckets[i].pokeKey()+",\n\t\t\t\t\t\"pokeName\": "+this.buckets[i].pokeName()+",\n\t\t\t\t\t\"pokePower\": "+this.buckets[i].pokePower()+",\n\t\t\t\t\t\"pokeType\": "+this.buckets[i].pokeType()+"\n\t\t\t\t}";
			if(i!=this.loadFactor-1)
				line+=(",\n");
		}
		return line;
	}
	protected CengPoke[] getBucketElements() {
		return this.buckets;
	}
	protected void increaseLocalDepth() {
		this.localDepth++;
	}
	protected void decreaseLocalDepth() {
		if (this.localDepth > 1) {
			this.localDepth--;
		}
	}

	public int insert(CengPoke poke)  {
		if (this.loadFactor < this.bucketSize) {
			this.buckets[this.loadFactor] = poke;
			this.loadFactor++;
			return this.loadFactor;
		}
		else
			return -1;
	}


	public int find(CengPoke poke) {
		if (this.bucketSize > 0) {
			for (int i = 0; i < this.loadFactor; i++) {
				if (poke.equals(this.buckets[i])) {
					return i;
				}
			}
			return -1;
		} else {
			return -1;
		}
	}

	    public void delete(int pokkey) {
		int wh=-1;

		for (int i = 0; i < this.loadFactor; i++) {
			if (this.buckets[i].pokeKey() == pokkey) {
				wh = i;
			}
		}

		CengPoke[] newBuckets = new CengPoke[this.bucketSize];
		for (int i = 0, j = 0; i < this.bucketSize; i++) {
			if (i != wh) {
				newBuckets[j] = this.buckets[i];
				j++;
			}
		}
            this.buckets = newBuckets;
            this.loadFactor--;
    }




	// GUI-Based Methods
	// These methods are required by GUI to work properly.
	
	public int pokeCount()
	{
		// TODO: Return the pokemon count in the bucket.
		return 0;		
	}
	
	public CengPoke pokeAtIndex(int index)
	{
		// TODO: Return the corresponding pokemon at the index.
		return null;
	}
	
	public int getHashPrefix()
	{
		// TODO: Return hash prefix length.
		return 0;
	}
	
	public Boolean isVisited()
	{
		// TODO: Return whether the bucket is found while searching.
		return false;		
	}
	
	// Own Methods
}
