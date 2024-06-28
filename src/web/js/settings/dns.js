const dnsenabled = document.querySelector('#dnsenable')
const setdnssettingsbutton = document.querySelector('#set_dns_settings')
const dnssettingbody = document.querySelector('#dnssettingsbody')
const dnsserverstablebody = document.querySelector('#dnsserverstablebody')

const adddnsserverbutton = document.querySelector('#add_dns_server_button')
const adddnsserverform = document.querySelector('#adddnsserverform')
const adddnsservercancelbutton = document.querySelector('#adddnsservercancel')
const adddnsserverconfirmbutton = document.querySelector('#adddnsserverconfirm')
const adddnsserverinput = document.querySelector('#adddnsserverinput')

let dnsservers = []

const get_dns_enabled_status = () => {
    fetch(routes.dns_status(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}"}`
    }).then(res => res.json()).then(jsondata => {
        console.log(jsondata)
        if (jsondata.enabled === '1') {
            dnsenabled.checked = true
            dnssettingbody.classList.add('active')
            adddnsserverbutton.classList.add('active')
            // adddnsserverbutton.parentElement.classList.remove('onebutton')
            return;
        } else {
            adddnsserverbutton.parentElement.classList.add('onebutton')
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
            adddnsserverbutton.classList.add('active')
        }
        notification("Настройки dns сохранены", "success")
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

const set_dns_settings = () => {
    set_dns_enabled_status()
    set_dns_server()
}

setdnssettingsbutton.addEventListener('click', () =>{
    get_confirm_form("Confirm DNS settings?", set_dns_settings)
})

dnsenabled.addEventListener('click', () => {
    if (dnsenabled.checked) {
        dnssettingbody.classList.add('active')
        adddnsserverbutton.classList.add('active')
        adddnsserverbutton.parentElement.classList.remove('onebutton')
        get_dns_server()
    } else {
        dnssettingbody.classList.remove('active')
        adddnsserverbutton.classList.remove('active')
        adddnsserverbutton.parentElement.classList.add('onebutton')
    }
})

const get_dns_server = () => {
    fetch(routes.dns_servers_get(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}"}`
    }).then(res => res.json()).then(jsondata => {
        console.log(jsondata)
        dnsservers = jsondata.servers
        dnsserverstablebody.innerHTML = ""
        for (const server in jsondata.servers) {
            dnsserverstablebody.innerHTML += `<tr><th>${jsondata.servers[server]} <button class="deldnsserverbutton" id="${jsondata.servers[server]}">Del</button></th></tr>`
        }
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

const set_dns_server = () => {
    start_loader("Load DNS")
    console.log(`{"token":"${localStorage.getItem('token')}","servers":${JSON.stringify(dnsservers)}}`)
    fetch(routes.dns_servers_set(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem('token')}","servers":${JSON.stringify(dnsservers)}}`
    }).then(data => data.json()).then(jsondata => {
        stop_loader()
        jsondata.success ? get_dns_server() : notification("Неизвестная ошибка", "error")
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
    adddnsserverinput.value = ""
}

const cancel_add_dns_server = () => {
    adddnsserverform.classList.remove('active')
    adddnsserverinput.value = ""
}

const cancel_confirm_add_dns_server = () => {
    confirmform.classList.remove('active')
    adddnsserverform.classList.add('active')
}

adddnsserverbutton.addEventListener('click', () => {
    adddnsserverform.classList.add('active')
})

adddnsservercancelbutton.addEventListener('click', cancel_add_dns_server)

adddnsserverconfirmbutton.addEventListener('click', () => {
    adddnsserverform.classList.remove('active')
    if (adddnsserverinput.value === "") {
        return
    }
    dnsservers.push(adddnsserverinput.value)
    get_confirm_form(`Confirm DNS settings? (Add DNS server: ${adddnsserverinput.value})`, set_dns_server, cancel_confirm_add_dns_server)
})

dnsserverstablebody.addEventListener('click', (e) => {
    if (e.target.classList.contains('deldnsserverbutton')) {
        dnsservers.splice(dnsservers.indexOf(e.target.id), 1)
        set_dns_server()
        console.log('del', e.target.id)
        get_dns_server()
    }
})
