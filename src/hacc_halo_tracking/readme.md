# Launching Jupyter Notebook

Remote:

```bash
jupyter notebook password # if first time
jupyter notebook --no-browser --port=8895
```

Locally:

```bash
ssh -N -f -L localhost:8891:localhost:8895 pascal@darwin-fe
```

Open a web browser: localhost:8891