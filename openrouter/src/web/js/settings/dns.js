const dnsenabled = document.querySelector('#dnsenable')
const setdnssettingsbutton = document.querySelector('#set_dns_settings')
const dnssettingbody = document.querySelector('#dnssettingsbody')
const dnsserverstablebody = document.querySelector('#dnsserverstablebody')

const adddnsserverbutton = document.querySelector('#add_dns_server_button')
const adddnsserverform = document.querySelector('#adddnsserverform')
const adddnsservercancelbutton = document.querySelector('#adddnsservercancel')
const adddnsserverconfirmbutton = document.querySelector('#adddnsserverconfirm')
const adddnsserverinput = document.querySelector('#adddnsserverinput')

const localdomainstablebody = document.querySelector('#domainsstablebody')
const addlocaldomainbutton = document.querySelector('#add_local_domain_button')
const addlocaldomainform = document.querySelector('#addlocaldomainform')
const addlocaldomaincancelbutton = document.querySelector('#addlocaldomaincancel')
const addlocaldomainconfirmbutton = document.querySelector('#addlocaldomainconfirm')
const domaininput = document.querySelector('#domaininput')
const ipinput = document.querySelector('#ipinput')
const domainstablebody = document.querySelector('#domainsstablebody')

let dnsservers = []
let localdomains = []

const get_dns_enabled_status = () => {
    fetch(routes.dns_status(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}"}`
    }).then(res => res.json()).then(jsondata => {
        if (jsondata.enabled === '1') {
            dnsenabled.checked = true
            dnssettingbody.classList.add('active')
            adddnsserverbutton.classList.add('active')
            addlocaldomainbutton.classList.add('active')
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
        body: `{"token":"${localStorage.getItem("token")}","enabled":"${dnsenabled.checked ? 1 : 0}"}`
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
    if (dnsenabled.checked) {
        set_dns_server()
        set_local_domains()
    }
}

setdnssettingsbutton.addEventListener('click', () => {
    if (confirm("Confirm DNS settings?")) {
        set_dns_settings()
    }
})

dnsenabled.addEventListener('click', () => {
    if (dnsenabled.checked) {
        dnssettingbody.classList.add('active')
        adddnsserverbutton.classList.add('active')
        addlocaldomainbutton.classList.add('active')
        adddnsserverbutton.parentElement.classList.remove('onebutton')
        get_dns_server()
    } else {
        dnssettingbody.classList.remove('active')
        adddnsserverbutton.classList.remove('active')
        addlocaldomainbutton.classList.remove('active')
        adddnsserverbutton.parentElement.classList.add('onebutton')
    }
})

const get_dns_server = () => {
    fetch(routes.dns_servers_get(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem("token")}"}`
    }).then(res => res.json()).then(jsondata => {
        dnsservers = jsondata.servers
        dnsserverstablebody.innerHTML = ""
        for (const server in jsondata.servers) {
            dnsserverstablebody.innerHTML += `<tr>
                    <th class="one">
                        ${jsondata.servers[server]}
                        <button class="deldnsserverbutton" id="${jsondata.servers[server]}">Del</button>
                    </th>
            </tr>`
        }
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

const set_dns_server = () => {
    start_loader("Load DNS")
    dnsservers.push(adddnsserverinput.value)
    fetch(routes.dns_servers_set(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem('token')}","servers":${JSON.stringify(dnsservers)}}`
    }).then(data => data.json()).then(jsondata => {
        stop_loader()
        if (jsondata.success) {
            get_dns_server()
            notification("Настройки dns сохранены", "success")
        } else {
            notification("Неизвестная ошибка", "error")
        }
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
    if (confirm(`Confirm DNS settings? (Add DNS server: ${adddnsserverinput.value})`)) {
        set_dns_server()
    } else {
        cancel_confirm_add_dns_server
    }
})

dnsserverstablebody.addEventListener('click', (e) => {
    if (e.target.classList.contains('deldnsserverbutton')) {
        dnsservers.splice(dnsservers.indexOf(e.target.id), 1)
        if (confirm(`Confirm DNS settings? (delete DNS server: ${e.target.id})`)) {
            set_dns_server()
        }
        get_dns_server()
    }
})

// ---------------------- local domains ---------------------------

const get_local_domains = () => {
    fetch(routes.dns_local_domains_get(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem('token')}"}`
    }).then(data => data.json()).then(jsondata => {
        localdomains = jsondata.domains
        localdomainstablebody.innerHTML = ""
        for (const domain in jsondata.domains) {
            localdomainstablebody.innerHTML += `<tr>
                            <th class="three">${jsondata.domains[domain].ip}</th>
                            <th class="three" style="display:flex;align-items:center;justify-content:space-between;width:100%">
                                ${jsondata.domains[domain].domain}
                                <button class="deldomain" id="${jsondata.domains[domain].domain}">Del</button>
                            </th>
            </tr>`
        }
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

const set_local_domains = (domains = localdomains) => {
    start_loader()
    fetch(routes.dns_local_domains_set(), {
        method: 'POST',
        body: `{"token":"${localStorage.getItem('token')}","domains":${JSON.stringify(domains)}}`
    }).then(data => data.json()).then(jsondata => {
        stop_loader()
        if (jsondata.success) {
            get_local_domains()
            notification("Настройки dns сохранены", "success")
        } else {
            notification("Неизвестная ошибка", "error")
        }
    }).catch((error) => {
        notification(`Ошибка на сервере: ${error}`, "error")
    })
}

const cancel_confirm_add_localdomain = () => {
    confirmform.classList.remove('active')
    addlocaldomainform.classList.add('active')
}

addlocaldomainbutton.addEventListener('click', () => {
    addlocaldomainform.classList.add('active')
})

addlocaldomaincancelbutton.addEventListener('click', () => {
    addlocaldomainform.classList.remove('active')
    domaininput.value = "";
    ipinput.value = "";
})

addlocaldomainconfirmbutton.addEventListener('click', () => {
    addlocaldomainform.classList.remove('active')
    if (confirm(`Confirm DNS settings? (Add Local Domain: ${ipinput.value} ${domaininput.value})`)) {
        localdomains.push({ "ip": ipinput.value, "domain": domaininput.value });
        set_local_domains()
        ipinput.value = ""
        domaininput.value = ""
    } else {
        cancel_confirm_add_localdomain()
    }
})

domainstablebody.addEventListener('click', (e) => {
    if (e.target.classList.contains('deldomain')) {
        localdomains.splice(localdomains.indexOf(localdomains.find(dom => dom.domain === e.target.id)), 1)
        if (confirm(`Confirm DNS settings? (delete Local Domain: ${e.target.id})`)) {
            set_local_domains()
        }
        get_local_domains()
    }
})


// ------------------------- local domains end --------------------
