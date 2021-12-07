class UI {
  constructor() {
    // this.trianglify = window.trianglify;
    // this.canvas = document.getElementById('main-canvas');

    // this.updateTriangles();
  }

  // updateTriangles() {
  //   let userValues = this.getUserValues();
  //   userValues['f0'] = userValues['f0'] / 100;
  //   let params = {
  //     width: window.innerWidth * 0.82,
  //     height: window.innerHeight * 0.4,
  //     variance: 1 - (userValues['noteSustainTime'] / 10000),
  //     seed: (userValues['noteFadeInTime'] + userValues['noteFadeOutTime']) / 20000
  //   };
  //   let pattern = this.trianglify(params);

  //   pattern.toCanvas(this.canvas);
  // }

  showAddresses(accountAddress, contractAddress) {
    document.getElementById('connected_account').innerHTML = accountAddress;
    document.getElementById('connected_contract').innerHTML = contractAddress;

    document.getElementById('connected_addresses').classList.remove('hidden');
    document.getElementById('hide_before_connect').classList.remove('hidden');
    document.getElementById('usage-instructions').classList.remove('hidden');
    document.getElementById('connect_button').classList.add('hidden');
    document.getElementById('connect-instructions').classList.add('hidden');
  }

  updateValues(currentState) {
    console.log(currentState);
    for (let key in currentState) {
      if (key == 'chordFamily') continue;

      let element = document.getElementById(`input_${key}`);
      if (element.tagName == 'INPUT') {
        element.value = currentState[key];
        document.getElementById(`output_${key}`).innerHTML = currentState[key];
      } else {
        console.log(element);
        element.selectedIndex = currentState[key] - 1;
      }
    }

    document.getElementById('input_chordFamily').selectedIndex = currentState['chordFamily'];
  }

  getUserValues() {
    let userValues = {
      f0: null,
      noteFadeInTime: null,
      noteSustainTime: null,
      noteFadeOutTime: null,
      chordProgressionStyle: null,
      chordPlayStyle: null,
      chordFamily: null
    }

    for (let key in userValues) {
      userValues[key] = document.getElementById(`input_${key}`).value;
    }
    userValues['f0'] = userValues['f0'] * 100;
    console.log(userValues);

    return userValues;
  }
}