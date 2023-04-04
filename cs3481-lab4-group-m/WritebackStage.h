//class to perform the combinational logic of
//the Fetch stage
class WritebackStage: public Stage
{
   private:
      void setEInput(W * ereg, uint64_t stat, uint64_t icode, uint64_t ifun, 
                    uint64_t valA, uint64_t valB, uint64_t valC, 
                    uint64_t valP, uint64_t dstE, uint64_t dstM,
                    uint64_t srcA, uint64_t srcB);
   public:
      bool doClockLow(PipeReg ** pregs, Stage ** stages);
      void doClockHigh(PipeReg ** pregs);

};