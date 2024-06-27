const dnsenabled = document.querySelector('#dnsenable')
const setdnssettingsbutton = document.querySelector('#set_dns_settings')
const dnssettingbody = document.querySelector('#dnssettingsbody')
const dnsserverstablebody = document.querySelector('#dnsserverstablebody')

const get_dns_enabled_status = () => {
    fetch(routes.dns_status(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}"}`
    }).then(res => res.json()).then(jsondata => {
        console.log(jsondata)
        if (jsondata.enabled === '1') {
            dnsenabled.checked = true
            dnssettingbody.classList.add('active')
            return;
        }
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

const set_dns_enabled_status = () => {
    fetch(routes.dns_status_set(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}","enabled":"${dnsenabled.checked?1:0}"}`
    }).then(res => res.json()).then(jsondata => {
        if (jsondata.enabled === '1') {
            dnsenabled.checked = true
            dnssettingbody.classList.add('active')
        }
        notification("Настройки dns сохранены", "success")
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

setdnssettingsbutton.addEventListener('click', () =>{
    get_confirm_form("Confirm dns settings?", set_dns_enabled_status)
})

dnsenabled.addEventListener('click', () => {
    if (dnsenabled.checked) {
        dnssettingbody.classList.add('active')
        get_dns_server()
    } else {
        dnssettingbody.classList.remove('active')
    }
})

const get_dns_server = () => {
    fetch(routes.dns_servers_get(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}"}`
    }).then(res => res.json()).then(jsondata => {
        console.log(jsondata)
        for (const server in jsondata.servers) {
            dnsserverstablebody.innerHTML += `<tr><th>${jsondata.servers[server]}</th></tr>`
        }
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}
