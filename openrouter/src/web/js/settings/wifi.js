const wifistatus = document.querySelector('#wifistatus')
const wifisettingsbody = document.querySelector('#wifisettingsbody')
const wifissid = document.querySelector('#SSID')
const wifipassword = document.querySelector('#wifipassword')
const showpasswordbutton = document.querySelector('#showpasswordbutton')
const setwifisettings = document.querySelector('#setwifisettings')
const shownewpasswordbutton = document.querySelector('#shownewpasswordbutton')
const newwifipassword = document.querySelector('#newwifipassword')
const wifiqr = document.querySelector('#wifiqr')
const wifiqrimg = document.querySelector("#wifiqrimg")
const channelselect = document.querySelector('#wifichannel')

let old_wifi_name = ""

const get_wifi_status = () => {
    fetch(routes.wifi_status_get(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem('token')}"}`
    }).then(data => data.json()).then(jsondata => {
        if (jsondata.status == "1") {
            wifistatus.checked = true
            wifisettingsbody.classList.add('active')
        } else {
            wifistatus.checked = false
        }
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

const set_wifi_status = () => {
    fetch(routes.wifi_status_set(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem('token')}","status":"${wifistatus.checked?"1":"0"}"}`
    }).then(data => data.json()).then(jsondata => {
        if (jsondata.status === "1") {
            wifistatus.checked = true
            wifisettingsbody.classList.add('active')
            notification("Настройки WIFI оспешно сорранены", "success")
            return
        }
        wifistatus.checked = false
        wifisettingsbody.classList.remove('active')
        notification("Настройки WIFI оспешно сорранены", "success")
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

const get_wifi_name = () => {
    fetch(routes.wifi_name_get(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem('token')}"}`
    }).then(data => data.json()).then(jsondata => {
        wifissid.value = jsondata.name
        old_wifi_name = jsondata.name
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

const set_wifi_name = () => {
    fetch(routes.wifi_name_set(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem('token')}","name":"${wifissid.value}"}`
    }).then(data => data.json()).then(jsondata => {
        wifissid.value = jsondata.name
        old_wifi_name = jsondata.name
        notification("Имя WIFI оспешно сорранено", "success")
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

const get_wifi_password = () => {
    fetch(routes.wifi_password_get(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem('token')}"}`
    }).then(data => data.json()).then(jsondata => {
        wifipassword.value = jsondata.password
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

const set_wifi_password = () => {
    fetch(routes.wifi_password_set(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem('token')}","password":"${wifipassword.value}","newpassword":"${newwifipassword.value}"}`
    }).then(data => data.json()).then(jsondata => {
        if (jsondata.status === "reject") {
            notification("Неверный пароль", "error")
            return
        }
        notification("Пароль WIFI оспешно сорранен", "success")
        wifipassword.value = ""
        newwifipassword.value = ""
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

const set_wifi_settings = () => {
    if (confirm("Confirm wifi settings?")) {
        start_loader()
        set_wifi_status()
        if (wifistatus.checked) {
            set_wifi_channel()
            if (old_wifi_name !== wifissid.value) {
                set_wifi_name()
            }
            if (wifipassword.value !== "" && newwifipassword.value !== "") {
                set_wifi_password()
            }
        }
        stop_loader()
    }
}

setwifisettings.addEventListener('click', set_wifi_settings)

wifistatus.addEventListener('click', () => {
    if (wifistatus.checked) {
        wifisettingsbody.classList.add('active')
        return
    }
    wifisettingsbody.classList.remove('active')
})

showpasswordbutton.addEventListener('click', () => {
    if (wifipassword.type == "password") {
        wifipassword.type = "text"
        return
    }
    wifipassword.type = "password"
})

shownewpasswordbutton.addEventListener('click', () => {
    if (newwifipassword.type == "password") {
        newwifipassword.type = "text"
        return
    }
    newwifipassword.type = "password"
})

const get_wifi_qr_code = () => {
    fetch(routes.wifi_qr_get(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem('token')}"}`
    }).then(data => data.text()).then(svgdata => {
        wifiqrimg.innerHTML = svgdata
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

wifiqr.addEventListener('click', () => {
    if (wifiqr.checked) {
        get_wifi_qr_code()
        wifiqrimg.innerHTML = 
        wifiqrimg.classList.add('active')
        return
    }
    wifiqrimg.classList.remove('active')
    wifiqrimg.innerHTML = ""
})

const get_wifi_channel = () => {
    fetch(routes.wifi_channel_get(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem('token')}"}`
    }).then(data => data.json()).then(jsondata => {
        channelselect.value = jsondata.channel
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

const set_wifi_channel = () => {
    fetch(routes.wifi_channel_set(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem('token')}","channel":"${channelselect.value}"}`
    }).then(data => data.json()).then(jsondata => {
        channelselect.value = jsondata.channel
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
} 
