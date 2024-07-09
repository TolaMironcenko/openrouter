const wifistatus = document.querySelector('#wifistatus')
const wifisettingsbody = document.querySelector('#wifisettingsbody')
const wifissid = document.querySelector('#SSID')
const wifipassword = document.querySelector('#wifipassword')

const get_wifi_status = () => {
    fetch(routes.wifi_status_get(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem('token')}"}`
    }).then(data => data.json()).then(jsondata => {
        console.log(jsondata)
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

const get_wifi_name = () => {
    fetch(routes.wifi_name_get(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem('token')}"}`
    }).then(data => data.json()).then(jsondata => {
        console.log(jsondata)
        wifissid.value = jsondata.name
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

const get_wifi_password = () => {
    fetch(routes.wifi_password_get(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem('token')}"}`
    }).then(data => data.json()).then(jsondata => {
        console.log(jsondata)
        wifipassword.value = jsondata.password
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

wifistatus.addEventListener('click', () => {
    if (wifistatus.checked) {
        wifisettingsbody.classList.add('active')
        return
    }
    wifisettingsbody.classList.remove('active')
})
