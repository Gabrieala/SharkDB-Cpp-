const DEFAULT_SHARKS = [
    {
        name: "Great White Shark",
        description: "Most recognizable and formidable predators in the ocean. Known for its sharp teeth and powerful build.",
        habitat: "Coastal and offshore waters",
        diet: "Sea turtles, seals, sea lions, porpoises, dolphins, and small whales",
        status: "Vulnerable",
        fact: "Can reach speeds of up to 56 km/h when in pursuit of prey."
    },
    {
        name: "Bull Shark",
        description: "Aggressive and adaptable, known for their ability to live in both salt and freshwater.",
        habitat: "Warm, shallow coastal waters and rivers",
        diet: "Fish, dolphins, turtles, and other sharks",
        status: "Near Threatened",
        fact: "Has some of the highest testosterone levels of any animal, which may contribute to their aggression."
    },
    {
        name: "Goblin Shark",
        description: "Rare, deep-sea species with an unusual appearance and long snout, often referred to as a living fossil.",
        habitat: "Deep waters, continental slopes, and underwater canyons",
        diet: "Fish, crustaceans, and squid",
        status: "Least Concern",
        fact: "Translucent skin makes their blood vessels visible, giving them a pinkish hue."
    },
    {
        name: "Nurse Shark",
        description: "Slow-moving bottom-dwellers that primarily hunt at night.",
        habitat: "Warm, shallow waters near reefs, commonly in the Atlantic and Pacific Oceans",
        diet: "Small fish, crustaceans, and mollusca",
        status: "Vulnerable",
        fact: "Docile and popular in underwater eco-tourism."
    },
    {
        name: "Whale Shark",
        description: "Largest fish in the sea, with distinctive white spots on a gray body. It's a gentle giant known for its slow and calm swimming behavior.",
        habitat: "Warm, tropical oceans around the world, often near coastlines and open waters",
        diet: "Filter-feeder; consumes plankton, small fish, and krill",
        status: "Endangered",
        fact: "Can grow up to 60 feet long, roughly the size of a school bus."
    },
    {
        name: "Basking Shark",
        description: "Second-largest fish in the world, known for their large, gaping mouths.",
        habitat: "Coastal and open ocean, primarily in temperate regions",
        diet: "Plankton, filter-fed through their large mouths",
        status: "Endangered",
        fact: "Harmless to humans, feeding only on small organisms."
    },
    {
        name: "Lemon Shark",
        description: "Named for their yellowish color, Lemon Sharks blend well with sandy environments. They are social and intelligent.",
        habitat: "Shallow coastal waters, mainly in subtropical regions like the Atlantic coast",
        diet: "Small fish, crustaceans, and mollusca",
        status: "Near Threatened",
        fact: "Social animals often found in groups, which is rare for most shark species."
    },
    {
        name: "Greenland Shark",
        description: "Longest-living vertebrates. They thrive in cold, deep-sea habitats.",
        habitat: "North Atlantic and Arctic Oceans, usually at great depths",
        diet: "Fish, carrion, and marine mammals",
        status: "Near Threatened",
        fact: "Lifespans estimated to be over 400 years; often parasitized by a small crustacean that attaches to their eyes, rendering many partially blind."
    },
    {
        name: "Angel Shark",
        description: "Flat-bodied with broad pectoral fins and sandy camouflage.",
        habitat: "Coastal waters, seabeds, and continental shelves of the eastern Atlantic and Mediterranean Sea",
        diet: "Fish, molluscs, and crustaceans",
        status: "Critically Endangered",
        fact: "Masters of disguise that blend with the ocean floor and look like rays or patches of sand to ambush prey."
    }
];

const STORAGE_KEY = "sharkdb_sharks";

function loadSharks() {
    try {
        const saved = localStorage.getItem(STORAGE_KEY);
        if (saved) {
            return JSON.parse(saved);
        }
    } catch (err) {
        console.error("Failed to read saved sharks:", err);
    }
    return DEFAULT_SHARKS.slice();
}

function saveSharks(sharks) {
    localStorage.setItem(STORAGE_KEY, JSON.stringify(sharks));
}

function escapeHtml(text) {
    const div = document.createElement("div");
    div.textContent = text;
    return div.innerHTML;
}

function getBadgeClass(status) {
    const normalized = status.trim().toLowerCase();
    if (normalized === "least concern") return "badge-least-concern";
    if (normalized === "near threatened") return "badge-near-threatened";
    if (normalized === "vulnerable") return "badge-vulnerable";
    if (normalized === "endangered") return "badge-endangered";
    if (normalized === "critically endangered") return "badge-critically-endangered";
    return "badge-least-concern";
}

function createSharkCard(shark) {
    const card = document.createElement("article");
    card.className = "shark-card";
    card.innerHTML = `
        <div class="card-top">
            <h3>${escapeHtml(shark.name)}</h3>
            <span class="badge ${getBadgeClass(shark.status)}">${escapeHtml(shark.status)}</span>
        </div>
        <p><span class="label">Description:</span> ${escapeHtml(shark.description)}</p>
        <p><span class="label">Habitat:</span> ${escapeHtml(shark.habitat)}</p>
        <p><span class="label">Diet:</span> ${escapeHtml(shark.diet)}</p>
        <div class="fact-box"><span class="label">Fun Fact:</span> ${escapeHtml(shark.fact)}</div>
    `;
    return card;
}

let sharks = loadSharks();

const sharkListEl = document.getElementById("shark-list");
const sharkCountEl = document.getElementById("shark-count");
const statusFilterEl = document.getElementById("status-filter");
const searchFormEl = document.getElementById("search-form");
const searchInputEl = document.getElementById("search-input");
const searchResultEl = document.getElementById("search-result");
const addFormEl = document.getElementById("add-form");
const addMessageEl = document.getElementById("add-message");
const resetBtnEl = document.getElementById("reset-btn");

function renderAllSharks() {
    const filter = statusFilterEl.value;
    const filtered = filter === "all"
        ? sharks
        : sharks.filter(s => s.status.trim().toLowerCase() === filter);

    sharkCountEl.textContent = filtered.length;
    sharkListEl.innerHTML = "";

    if (filtered.length === 0) {
        sharkListEl.innerHTML = `<p class="empty-msg">No sharks match this conservation status.</p>`;
        return;
    }

    filtered.forEach(shark => {
        sharkListEl.appendChild(createSharkCard(shark));
    });
}

function runSearch() {
    const query = searchInputEl.value.trim().toLowerCase();
    searchResultEl.innerHTML = "";

    if (!query) {
        searchResultEl.innerHTML = `<p class="empty-msg">Type a shark name to search.</p>`;
        return;
    }

    const matches = sharks.filter(s => s.name.toLowerCase().includes(query));
    if (matches.length === 0) {
        searchResultEl.innerHTML = `<p class="empty-msg">No shark named '${escapeHtml(searchInputEl.value.trim())}' found in the database.</p>`;
        return;
    }

    const grid = document.createElement("div");
    grid.className = "shark-grid";
    matches.forEach(shark => grid.appendChild(createSharkCard(shark)));
    searchResultEl.appendChild(grid);
}

document.querySelectorAll(".tab-btn").forEach(btn => {
    btn.addEventListener("click", () => {
        document.querySelectorAll(".tab-btn").forEach(b => b.classList.remove("active"));
        document.querySelectorAll(".tab-panel").forEach(p => p.classList.remove("active"));
        btn.classList.add("active");
        document.getElementById("tab-" + btn.dataset.tab).classList.add("active");
    });
});

statusFilterEl.addEventListener("change", renderAllSharks);

searchFormEl.addEventListener("submit", e => {
    e.preventDefault();
    runSearch();
});

searchInputEl.addEventListener("input", () => {
    if (searchInputEl.value.trim() === "") {
        searchResultEl.innerHTML = "";
    } else {
        runSearch();
    }
});

addFormEl.addEventListener("submit", e => {
    e.preventDefault();

    const newShark = {
        name: document.getElementById("shark-name").value.trim(),
        description: document.getElementById("shark-description").value.trim(),
        habitat: document.getElementById("shark-habitat").value.trim(),
        diet: document.getElementById("shark-diet").value.trim(),
        status: document.getElementById("shark-status").value,
        fact: document.getElementById("shark-fact").value.trim()
    };

    sharks.push(newShark);
    saveSharks(sharks);
    renderAllSharks();
    addFormEl.reset();
    addMessageEl.textContent = `Shark '${newShark.name}' added successfully.`;
});

resetBtnEl.addEventListener("click", () => {
    localStorage.removeItem(STORAGE_KEY);
    sharks = DEFAULT_SHARKS.slice();
    renderAllSharks();
    addMessageEl.textContent = "Database reset to default sharks.";
});

renderAllSharks();
