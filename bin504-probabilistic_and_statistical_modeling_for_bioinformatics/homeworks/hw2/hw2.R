# Derya TINMAZ 2380947 BIN504 HW2
library(hgu133plus2.db)

# T-test is appropriate when comparing the means of two independent groups to determine 
# if there is a statistically significant difference between them. Therefore, I used t-test in the homework.

# loading the data
male_smokers <- read.csv("Male_Smoker.csv", row.names = 1)
male_nonsmokers <- read.csv("Male_NonSmoker.csv", row.names = 1)
female_smokers <- read.csv("Female_Smoker.csv", row.names = 1)
female_nonsmokers <- read.csv("Female_NonSmoker.csv", row.names = 1)

# taking transpose of the data, rows are patients, columns are probs now
male_smokers <- as.data.frame(t(male_smokers))
male_nonsmokers <- as.data.frame(t(male_nonsmokers))
female_smokers <- as.data.frame(t(female_smokers))
female_nonsmokers <- as.data.frame(t(female_nonsmokers))
combined_smokers <- rbind(male_smokers, female_smokers)
combined_nonsmokers <- rbind(male_nonsmokers, female_nonsmokers)

# initializing p values
p_values_males <- numeric(ncol(male_smokers))
p_values_females <- numeric(ncol(female_smokers))
p_values_combined <- numeric(ncol(combined_smokers))

probe_ids <- colnames(male_smokers)

# male smokers vs. male non-smokers
for (i in 1:ncol(male_smokers)) {
  p_values_males[i] <- t.test(male_smokers[,i], male_nonsmokers[,i])$p.value
}

# female smokers vs. female non-smokers
for (j in 1:ncol(female_smokers)) {
  p_values_females[j] <- t.test(female_smokers[,j], female_nonsmokers[,j])$p.value
}

# all smokers vs. all non-smokers
for (k in 1:ncol(combined_smokers)) {
  p_values_combined[k] <- t.test(combined_smokers[,k], combined_nonsmokers[,k])$p.value
}

#binding the results with corresponding probe ids
males_results <- cbind(probe_ids, p_values_males)
females_results <- cbind(probe_ids, p_values_females)
combined_results <- cbind(probe_ids, p_values_combined)

# initializing for the anova results
anova_results <- list()

# adding a group column to each data set to label the data, and binding all
male_smokers$group <- 'Male Smokers'
male_nonsmokers$group <- 'Male Non-Smokers'
female_smokers$group <- 'Female Smokers'
female_nonsmokers$group <- 'Female Non-Smokers'
all_data <- rbind(male_smokers, male_nonsmokers, female_smokers, female_nonsmokers)

# performing anova for each prob
for (i in 1:(ncol(all_data) - 1)) {
  probe_data <- all_data[, c(i, ncol(all_data))]
  names(probe_data) <- c("value", "group")
  anova_results[[probe_ids[i]]] <- aov(value ~ group, data = probe_data)
}

# number of probes that are significantly differentially expressed in each case
significant_males <- sum(p_values_males < 0.05)  # 3488
significant_females <- sum(p_values_females < 0.05) # 5004
significant_combined <- sum(p_values_combined < 0.05) # 8033
significant_probes_anova <- 0 # 4821
for (i in 1:length(anova_results)) {
  p_value <- summary(anova_results[[i]])[[1]]$'Pr(>F)'[1]
  if (p_value < 0.05) {
    significant_probes_anova <- significant_probes_anova + 1
  }
}

# perform chi-square test to check the difference of the findings
total_probes <- length(probe_ids)
non_significant_males <- total_probes - significant_males
non_significant_females <- total_probes - significant_females
non_significant_combined <- total_probes - significant_combined
non_significant_anova <- total_probes - significant_probes_anova

contingency_table <- matrix(c(significant_males, non_significant_males,
                              significant_females, non_significant_females,
                              significant_combined, non_significant_combined, 
                              significant_probes_anova, non_significant_anova),
                            nrow = 4, byrow = TRUE)
# naming the rows and columns
dimnames(contingency_table) <- list(c("Males", "Females", "Combined", "ANOVA"), c("Significant", "Non-significant"))
#             Significant    Non-significant
# Males           3488           51187
# Females         5004           49671
# Combined        8033           46642
# ANOVA           4821           49854

chi_square_test <- chisq.test(contingency_table)
# X-squared = 2297.6, df = 3, p-value < 2.2e-16
# The p-value of almost zero indicates a highly statistically significant difference among the four groups 
# in terms of the distribution of significant and non-significant outcomes.

# Bonferroni correction for the combined p values
bonferroni_adjusted_p_values <- p.adjust(p_values_combined, method = "bonferroni")
significant_probes_bonferroni <- probe_ids[bonferroni_adjusted_p_values < 0.05] # 21

# Benjamini-Hochberg correction for the combined p values
bh_adjusted_p_values <- p.adjust(p_values_combined, method = "BH")
significant_probes_bh <- probe_ids[bh_adjusted_p_values < 0.05] # 427

# to look up the gene name for a given probe
x <- hgu133plus2SYMBOL
mapped_probes <- mappedkeys(x)
x <- as.list(x[mapped_probes])

# getting the gene names of the corrected probs
bonferroni_corrected_genes <- as.character(sapply(significant_probes_bonferroni, function(probe) x[[probe]]))
bh_corrected_genes <- as.character(sapply(significant_probes_bh, function(probe) x[[probe]]))
bonferroni_corrected_genes

# from Table 1: Differentially expressed genes in the oral mucosa of smokers vs. never smokers
paper_genes <-as.character( unique(c("S100A7", "RPTN", "CYP1B1", "CYP1B1", "LOR", "CEACAM7", "CYP1A1", "CYP1B1", "HTR3A", "GPX2", "FCGBP", "CCL26","PNLIPRP3", "ALOX12B", "LOC388610", "CD207", "CHRNA3", 
  "CYTL1", "NQO1", "NQO1", "NQO1", "CLEC7A", "CLEC7A", "LOC344887", "PTGES", "KRT10", "C10orf99", "C10orf99", "ALDH3A1", "ALOX15B", "UGT1A6", "UGT1A8", "UGT1A9", "MUC1", "AKR1C1", "AKR1C2", "AHRR", "PDCD6", "LYPD5", 
  "UGT1A1", "UGT1A3", "UGT1A10", "CCL5", "UGT1A1", "UGT1A4", "UGT1A6", "UGT1A8", "UGT1A10", "CD1a", "LYVE1", "YOD1", "CCL18", "ANKRD37", "SOX9", "SOX9", "LEPR", "LEPR", 
  "LEPR", "IGF2BP3", "IGF2BP3", "CCL18", "HIG2", "PEG3"
)))


(bonferroni_corrected_genes) # 21
(bh_corrected_genes) # 427
(paper_genes) # 48

# finding matches
matches_with_bonferroni_corrected_genes <- intersect(paper_genes, bonferroni_corrected_genes) # 5
matches_with_bh_corrected_genes <- intersect(paper_genes, bh_corrected_genes) # 19

# results
print(paste("Matches With Bonferroni Corrected Genes:", toString(matches_with_bonferroni_corrected_genes)))
# "Matches With Bonferroni Corrected Genes: CYP1B1, CYP1A1, CD207, CYTL1, ALDH3A1"
print(paste("Matches With Benjamini-Hochberg Corrected Genes:", toString(matches_with_bh_corrected_genes)))
# "Matches With Benjamini-Hochberg Corrected Genes: CYP1B1, CYP1A1, GPX2, FCGBP, PNLIPRP3, CD207, CHRNA3, CYTL1, NQO1, CLEC7A, PTGES, ALDH3A1, AKR1C1, AKR1C2, CCL5, LYVE1, SOX9, LEPR, IGF2BP3"

