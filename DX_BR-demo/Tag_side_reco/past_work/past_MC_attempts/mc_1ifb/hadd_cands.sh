#cd cand_had
#hadd -f BGx0_mixed.root mixed*BGx0*
#mv -f BGx0_mixed.root ../Combined_cand/.
#hadd -f BGx0_charged.root charged*BGx0*
#mv -f BGx0_charged.root ../Combined_cand/.
#hadd -f BGx0_uubar.root uubar*BGx0*
#mv -f BGx0_uubar.root ../Combined_cand/.
#hadd -f BGx0_ddbar.root ddbar*BGx0*
#mv -f BGx0_ddbar.root ../Combined_cand/.
#hadd -f BGx0_ssbar.root ssbar*BGx0*
#mv -f BGx0_ssbar.root ../Combined_cand/.
#hadd -f BGx0_ccbar.root ccbar*BGx0*
#mv -f BGx0_ccbar.root ../Combined_cand/.
#cd ..
#cd Combined_cand
#hadd -f BGx0.root BGx0_*
#cd ..


cd cand_had
hadd -f BGx1_mixed.root mixed*BGx1*
mv -f BGx1_mixed.root ../Combined_cand/.
hadd -f BGx1_charged.root charged*BGx1*
mv -f BGx1_charged.root ../Combined_cand/.
hadd -f BGx1_uubar.root uubar*BGx1*
mv -f BGx1_uubar.root ../Combined_cand/.
hadd -f BGx1_ddbar.root ddbar*BGx1*
mv -f BGx1_ddbar.root ../Combined_cand/.
hadd -f BGx1_ssbar.root ssbar*BGx1*
mv -f BGx1_ssbar.root ../Combined_cand/.
hadd -f BGx1_ccbar.root ccbar*BGx1*
mv -f BGx1_ccbar.root ../Combined_cand/.
cd ..
cd Combined_cand
hadd -f BGx1.root BGx1_*
cd ..
