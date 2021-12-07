class UI {
  constructor() {
    this.trianglify = window.trianglify || require('trianglify');
  }

  update_triangles(params) {

  }

  showAddresses(accountAddress, contractAddress) {
    document.getElementById('connected_account').innerHTML = accountAddress;
    document.getElementById('connected_contract').innerHTML = contractAddress;

    document.getElementById('connected_addresses').classList.remove('hidden');
    document.getElementById('hide_before_connect').classList.remove('hidden');
    document.getElementById('connect_button').classList.add('hidden');
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
    console.log(userValues);

    return userValues;
  }
}