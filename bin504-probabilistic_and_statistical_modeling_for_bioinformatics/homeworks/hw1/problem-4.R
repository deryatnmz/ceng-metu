fly_lifetimes <- scan("fly_lifetimes.txt")

# initializing parameters
lambda <- 1 / mean(fly_lifetimes) 
lambda_prime <- 2 / mean(fly_lifetimes)  #mutated life time is shorter
mutated_ratio <- 0.5

for (iter in 1:1000) {
  
  # expectation step: calculating the probability of each fly being from each distribution
  prob_mutation <- mutated_ratio * dexp(fly_lifetimes, rate = lambda_prime)
  prob_normal <- (1 - mutated_ratio) * dexp(fly_lifetimes, rate = lambda)
  total_prob <- prob_mutation + prob_normal
  gamma <- prob_mutation / total_prob
  
  # maximization step: updating the parameters
  lambda <- sum(1 - gamma) / sum(fly_lifetimes * (1 - gamma))
  lambda_prime <- sum(gamma) / sum(fly_lifetimes * gamma)
  mutated_ratio <- sum(gamma) / length(fly_lifetimes)
  
}

# Results
cat("Estimated λ:", lambda, "\n")  
# Estimated λ: 0.01379122
cat("Expected Lifetime of Normal Flies:", 1/lambda, "\n") 
# Expected Lifetime of Normal Flies: 72.50992
cat("Estimated λ':", lambda_prime, "\n") 
# Estimated λ': 0.04454269 
cat("Expected Lifetime of Mutated Flies:", 1/lambda_prime, "\n")
# Expected Lifetime of Mutated Flies: 22.45037 
cat("Estimated Ratio of Mutated Flies:", mutated_ratio, "\n")
# Estimated Ratio of Mutated Flies: 0.1728926 
