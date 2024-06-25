simulation_fuction_for_A <- function() {
  while (1) {
    #sampling one allele from each parent
    mother_allele <- sample(c("T", "t"), 1, replace = TRUE) 
    father_allele <- sample(c("T", "t"), 1, replace = TRUE)
    if(!(mother_allele == "t" & father_allele == "t")){
      # making sure that A is not homozygous for the disease, he would not be alive otherwise
      break
    }
  }
  if ((mother_allele == "T" & father_allele == "t") | (mother_allele == "t" & father_allele == "T")) {
    return("carrier")
  }
  else {
    return ("non-carrier")
  }
}
set.seed(100) #to make the simulation reproducible
num_simulations <- 10000
simulation_for_A <- replicate(num_simulations, simulation_fuction_for_A())
probability_carrier_A <- sum(simulation_for_A == "carrier") / num_simulations
cat("Estimated probability of A being a carrier:", probability_carrier_A, "\n")
# Estimated probability of A being a carrier: 0.6645 

carrier_father_count <-0 
carrier_father = FALSE
simulation_function_for_child <- function() {
  
  #sampling one allele from carrier mother B
  mother_allele <- sample(c("T", "t"), 1, replace = TRUE) 
  #sampling one gene from simulation of A
  father_genes <- sample(simulation_for_A, 1)

  if(father_genes =="carrier"){ #if A is carrier, sample one allele
    carrier_father = TRUE 
    father_allele <- sample(c("T", "t"), 1, replace = TRUE)
  }
  else { #if A is not carrier he is TT
    carrier_father = FALSE
    father_allele <- "T"
  }
  
  
  if (mother_allele == "t" & father_allele == "t") {
    return ("sick")
  }
  else{
    if(carrier_father){
      carrier_father_count <<- carrier_father_count + 1
      # counting carrier father when the child is healthy
    }
    return ("healthy")
  }
    
}
simulation_for_child <- replicate(num_simulations, simulation_function_for_child())
probability_sick_child <- sum(simulation_for_child == "sick") / num_simulations
cat("Estimated probability of child being sick:", probability_sick_child, "\n")
# Estimated probability of child being sick: 0.165 

# ratio of carrier father count when the child is healthy to all healthy child count
probability_carrier_A_given_child = carrier_father_count/sum(simulation_for_child == "healthy")
cat("Estimated probability of A being a carrier given that his first child is healthy:",probability_carrier_A_given_child , "\n")
# Estimated probability of A being a carrier given that his first child is healthy: 0.5952096 
