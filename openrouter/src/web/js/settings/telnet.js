const telnetenabled = document.querySelector('#telnetenable')
const telnetsettingbody = document.querySelector('.telnetsettingbody')
const settelnetsettingsbutton = document.querySelector('#set_telnet_settings')
// const addtelnetuserbutton = document.querySelector('#add_telnet_user_button')

const telnetportinput = document.querySelector('#telnetportinput')

const get_telnet_enabled_status = () => {
    fetch(routes.telnet_status(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}"}`
    }).then(res => res.json()).then(jsondata => {
        if (jsondata.enabled === '1') {
            telnetenabled.checked = true
            // addtelnetuserbutton.classList.add('active')
            // addtelnetuserbutton.parentElement.classList.remove('onebutton')
            telnetsettingbody.classList.add('active')
            return;
        } else {
            // addtelnetuserbutton.parentElement.classList.add('onebutton')
        }
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

const set_telnet_enabled_status = () => {
    fetch(routes.telnet_status_set(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}","enabled":"${telnetenabled.checked?1:0}"}`
    }).then(res => res.json()).then(jsondata => {
        if (jsondata.enabled === '1') {
            telnetenabled.checked = true
            // telnetsettingbody.classList.add('active')
        }
        notification("Настройки telnet сохранены", "success")
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

telnetenabled.addEventListener('click', () => {
    if (telnetenabled.checked) {
        // addtelnetuserbutton.classList.add('active')
        // addtelnetuserbutton.parentElement.classList.remove('onebutton')
        telnetsettingbody.classList.add('active')
    } else {
        telnetsettingbody.classList.remove('active')
        // addtelnetuserbutton.classList.remove('active')
        // addtelnetuserbutton.parentElement.classList.add('onebutton')
    }
})

const set_telnet_settings = () => {
    set_telnet_enabled_status()
    if (telnetenabled.checked) {
        set_telnet_port()
    }
}

settelnetsettingsbutton.addEventListener('click', () => {
    if (confirm("Confirm telnet settings?")) {
        set_telnet_settings()
    }
})

const get_telnet_port = () => {
    fetch(routes.telnet_port_get(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}"}`
    }).then(data => data.json()).then(jsondata => {
        telnetportinput.value = jsondata.port
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

const set_telnet_port = () => {
    start_loader()
    fetch(routes.telnet_port_set(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}","port":"${telnetportinput.value}"}`
    }).then(data => data.json()).then(jsondata => {
        stop_loader()
        telnetportinput.value = jsondata.port
    }).catch((error) => {
        stop_loader()
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}
