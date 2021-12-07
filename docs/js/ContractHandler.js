class ContractHandler {
  constructor() {
    this.isConnected = false;
    this.address = '0xD858DE24B61A0E9266D738940487d92991b72219';

    this.account = null;
    this.web3 = new Web3(Web3.givenProvider || "ws://localhost:8545");

    this.abi = JSON.parse('[{"inputs":[],"stateMutability":"nonpayable","type":"constructor"},{"inputs":[],"name":"getChordFamily","outputs":[{"internalType":"int32","name":"","type":"int32"}],"stateMutability":"view","type":"function"},{"inputs":[],"name":"getChordPlayStyle","outputs":[{"internalType":"int32","name":"","type":"int32"}],"stateMutability":"view","type":"function"},{"inputs":[],"name":"getChordProgressionStyle","outputs":[{"internalType":"int32","name":"","type":"int32"}],"stateMutability":"view","type":"function"},{"inputs":[],"name":"getF0","outputs":[{"internalType":"int32","name":"","type":"int32"}],"stateMutability":"view","type":"function"},{"inputs":[],"name":"getNoteFadeInTime","outputs":[{"internalType":"int32","name":"","type":"int32"}],"stateMutability":"view","type":"function"},{"inputs":[],"name":"getNoteFadeOutTime","outputs":[{"internalType":"int32","name":"","type":"int32"}],"stateMutability":"view","type":"function"},{"inputs":[],"name":"getNoteSustainTime","outputs":[{"internalType":"int32","name":"","type":"int32"}],"stateMutability":"view","type":"function"},{"inputs":[{"internalType":"int32","name":"newF0","type":"int32"},{"internalType":"int32","name":"newFadeInTime","type":"int32"},{"internalType":"int32","name":"newSustainTime","type":"int32"},{"internalType":"int32","name":"newFadeOutTime","type":"int32"},{"internalType":"int32","name":"newChordProgressionStyle","type":"int32"},{"internalType":"int32","name":"newChordPlayStyle","type":"int32"},{"internalType":"int32","name":"newChordFamily","type":"int32"}],"name":"setAll","outputs":[],"stateMutability":"nonpayable","type":"function"},{"inputs":[{"internalType":"int32","name":"newChordFamily","type":"int32"}],"name":"setChordFamily","outputs":[],"stateMutability":"nonpayable","type":"function"},{"inputs":[{"internalType":"int32","name":"newChordPlayStyle","type":"int32"}],"name":"setChordPlayStyle","outputs":[],"stateMutability":"nonpayable","type":"function"},{"inputs":[{"internalType":"int32","name":"newChordProgressionStyle","type":"int32"}],"name":"setChordProgressionStyle","outputs":[],"stateMutability":"nonpayable","type":"function"},{"inputs":[{"internalType":"int32","name":"newF0","type":"int32"}],"name":"setF0","outputs":[],"stateMutability":"nonpayable","type":"function"},{"inputs":[{"internalType":"int32","name":"newFadeInTime","type":"int32"}],"name":"setNoteFadeInTime","outputs":[],"stateMutability":"nonpayable","type":"function"},{"inputs":[{"internalType":"int32","name":"newFadeOutTime","type":"int32"}],"name":"setNoteFadeOutTime","outputs":[],"stateMutability":"nonpayable","type":"function"},{"inputs":[{"internalType":"int32","name":"newSustainTime","type":"int32"}],"name":"setNoteSustainTime","outputs":[],"stateMutability":"nonpayable","type":"function"}]');
    this.contract = null;

    this.currentState = {
      f0: null,
      noteFadeInTime: null,
      noteSustainTime: null,
      noteFadeOutTime: null,
      chordProgressionStyle: null,
      chordPlayStyle: null,
      chordFamily: null
    }
  }

  async connectMetamask() {
    try {
      let accounts = await window.ethereum.request({
        method: 'eth_requestAccounts'
      });
      this.account = accounts[0];
      console.log('attempt succesful');
      this.isAccountConnected = true;
    } catch (err) {
      alert("error connecting");
      console.error(err);
      return null;
    }
    return this.account;
  }

  async getCurrentState() {
    this.currentState['f0'] = Number(await this.contract.methods.getF0().call({from: this.account})) / 100;
    this.currentState['noteFadeInTime'] = Number(await this.contract.methods.getNoteFadeInTime().call({from: this.account}));
    this.currentState['noteSustainTime'] = Number(await this.contract.methods.getNoteSustainTime().call({from: this.account}));
    this.currentState['noteFadeOutTime'] = Number(await this.contract.methods.getNoteFadeOutTime().call({from: this.account}));
    this.currentState['chordProgressionStyle'] = Number(await this.contract.methods.getChordProgressionStyle().call({from: this.account}));
    this.currentState['chordPlayStyle'] = Number(await this.contract.methods.getChordPlayStyle().call({from: this.account}));
    this.currentState['chordFamily'] = Number(await this.contract.methods.getChordFamily().call({from: this.account}));

    return this.currentState;
  }

  async connectContract() {
    this.contract = await new this.web3.eth.Contract(this.abi, this.address); // connect to contract
    console.log(this.contract);

    await this.getCurrentState();
    
    console.log(this.currentState);
    return [this.address, this.currentState];
  }

  async submitValues(userValues) {
    await this.contract.methods.setAll(
      userValues['f0'], 
      userValues['noteFadeInTime'], 
      userValues['noteSustainTime'],
      userValues['noteFadeOutTime'],
      userValues['chordProgressionStyle'],
      userValues['chordPlayStyle'],
      userValues['chordFamily']
    ).send({from: this.account});
  }
}