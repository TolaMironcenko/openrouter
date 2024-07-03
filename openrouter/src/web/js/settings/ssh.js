const sshenable = document.querySelector('#sshenable')
const set_ssh_settings = document.querySelector('#set_ssh_settings')
const sshsettingsbody = document.querySelector('#sshsettingsbody')
const sshkeystablebody = document.querySelector('#sshkeystablebody')
const sshportinput = document.querySelector('#sshportinput')
const add_ssh_key_button = document.querySelector('#add_ssh_key')

let sshkeys = []

// -------------------- SSH status --------------------------------
const get_ssh_status = () => {
    fetch(routes.ssh_status_get(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}"}`
    }).then(data => data.json()).then(jsondata => {
        if (jsondata.status === '1') {
            sshenable.checked = true
            sshsettingsbody.classList.add('active')
            add_ssh_key_button.classList.add('active')
            add_ssh_key_button.parentElement.classList.remove('onebutton')
        }
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

const set_ssh_status = () => {
    fetch(routes.ssh_status_set(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}","status":"${sshenable.checked?1:0}"}`
    }).then(res => res.json()).then(jsondata => {
        if (jsondata.status === '1') {
            sshenable.checked = true
            // telnetsettingbody.classList.add('active')
        }
        notification("Настройки SSH сохранены", "success")
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}
// -------------------------------- end SSH status ---------------------------------------

set_ssh_settings.addEventListener('click', () => {
    if (confirm("Confirm SSH settings?")) {
        set_ssh_status()
        set_ssh_port()
    }
})

sshenable.addEventListener('click', () => {
    if (sshenable.checked) {
        sshsettingsbody.classList.add('active')
        add_ssh_key_button.classList.add('active')
        add_ssh_key_button.parentElement.classList.remove('onebutton')
        return
    }
    sshsettingsbody.classList.remove('active')
    add_ssh_key_button.classList.remove('active')
    add_ssh_key_button.parentElement.classList.add('onebutton')
})

// -------------------- SSH keys ---------------------------------
const get_ssh_keys = () => {
    fetch(routes.ssh_keys_get(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}"}`
    }).then(data => data.json()).then(jsondata => {
        console.log(jsondata.keys)
        sshkeys = jsondata.keys
        sshkeystablebody.innerHTML = ""
        for (const key in sshkeys) {
            sshkeystablebody.innerHTML += `<tr>
                    <td class="one" style="flex-direction:column;align-items:start;">
                        ${sshkeys[key]}
                        <button class="delsshkeybutton" id="${key}">Del</button>
                    </td>
            </tr>`
        }
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

const set_ssh_keys = () => {
    start_loader()
    fetch(routes.ssh_keys_set(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}","keys":${JSON.stringify(sshkeys)}}`
    }).then(data => data.json()).then(jsondata => {
        stop_loader()
        if (jsondata.success) {
            get_ssh_keys()
            notification("Настройки SSH сохранены", "success")
        } else {
            notification("Неизвестная ошибка", "error")
        }
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}
// ------------------- end SSH keys ------------------------------

sshkeystablebody.addEventListener('click', (e) => {
    if (e.target.classList.contains('delsshkeybutton')) {
        if (confirm(`Confirm SSH settings?: (delete SSH key)`)) {
            sshkeys.splice(e.target.id)
            set_ssh_keys()
        }
    }
})

add_ssh_key_button.addEventListener('click', () => {
    const sshkey = prompt('Get your SSH key:')
    if (confirm('Confirm SSH settings? (add SSH key)')) {
        sshkeys.push(sshkey)
        set_ssh_keys()
    }
})

// -------------------- SSH port ----------------------
const get_ssh_port = () => {
    fetch(routes.ssh_port_get(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}"}`
    }).then(data => data.json()).then(jsondata => {
        sshportinput.value = jsondata.port
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

const set_ssh_port = () => {
    fetch(routes.ssh_port_set(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}","port":"${sshportinput.value}"}`
    }).then(data => data.json()).then(jsondata => {
        sshportinput.value = jsondata.port
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}
// ------------------- end SSH port -------------------
