class App {
  constructor() {
    this.contractHandler = new ContractHandler();
    this.ui = new UI();
  }

  async connectMetamask() {
    let accountAddress = await this.contractHandler.connectMetamask();
    if (accountAddress == null) {
      return;
    }

    let [contractAddress, currentState] = await this.contractHandler.connectContract();

    this.ui.showAddresses(accountAddress, contractAddress);
    this.ui.updateValues(currentState);
  }

  changeSlider(val, id) {
    console.log(val);
    console.log(id);
  }

  async submit() {
    console.log("submit");
    let userValues = this.ui.getUserValues();

    await this.contractHandler.submitValues(userValues);

    alert('Your submission will take some time to take effect. You can check your status on Metamask');
    // await this.reset();
  }

  async reset() {
    console.log("reset");
    let currentState = await this.contractHandler.getCurrentState();
  
    this.ui.updateValues(currentState);
  }
}

var app = new App();