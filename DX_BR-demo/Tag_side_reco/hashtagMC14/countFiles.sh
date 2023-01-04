# Write similar lines to double check other mcTypes
# [Although already checked by missingOutput.py]
echo 'skimOut'
ls hashtagJan08_output/skimOut_2_* hashtagJan08_output/skimOut_8_* hashtagJan08_output/skimOut_14_* hashtagJan08_output/skimOut_20_*|wc -l
echo 'decFlag'
ls hashtagJan08_output/decFlag_2_* hashtagJan08_output/decFlag_8_* hashtagJan08_output/decFlag_14_* hashtagJan08_output/decFlag_20_*|wc -l

#ls hashtagJan08_output/decFlag_2_* hashtagJan08_output/decFlag_8_* hashtagJan08_output/decFlag_14_* hashtagJan08_output/decFlag_20_* > myList.txt
#ls hashtagJan08_output/skimOut_2_* hashtagJan08_output/skimOut_8_* hashtagJan08_output/skimOut_14_* hashtagJan08_output/skimOut_20_* > myList.txt
