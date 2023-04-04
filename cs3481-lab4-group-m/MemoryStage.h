//class to perform the combinational logic of
//the Fetch stage
class MemoryStage: public Stage
{
    private:
       void setWInput(W * wreg, uint64_t stat, uint64_t icode, 
                     uint64_t valE, uint64_t valM, uint64_t dstE,
                     uint64_t dstM);
    public:
       bool doClockLow(PipeReg ** pregs, Stage ** stages);
       void doClockHigh(PipeReg ** pregs);

};
