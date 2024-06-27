const telnetenabled = document.querySelector('#telnetenable')
const telnetsettingbody = document.querySelector('.telnetsettingbody')
const settelnetsettingsbutton = document.querySelector('#set_telnet_settings')

const get_telnet_enabled_status = () => {
    fetch(routes.telnet_status(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}"}`
    }).then(res => res.json()).then(jsondata => {
        console.log(jsondata)
        if (jsondata.enabled === '1') {
            telnetenabled.checked = true
            // telnetsettingbody.classList.add('active')
            return;
        }
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

get_telnet_enabled_status()

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
        // telnetsettingbody.classList.add('active')
    } else {
        telnetsettingbody.classList.remove('active')
    }
})

settelnetsettingsbutton.addEventListener('click', () => {
    get_confirm_form("Confirm telnet settings?", set_telnet_enabled_status)
})
